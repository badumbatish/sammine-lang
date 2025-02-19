#pragma once
#include "fmt/base.h"
#include "fmt/color.h"
#include "fmt/core.h"
#include <__algorithm/ranges_lower_bound.h>
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ranges>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace sammine_util {
auto get_string_from_file(const std::string &file_name) -> std::string;
inline size_t unique_ast_id = 0;

[[noreturn]]
auto abort(const std::string &message = "<NO MESSAGE>") -> void;
auto abort_on(bool abort_if_true, const std::string &message = "<NO MESSAGE>")
    -> void;
//! A class representing a location for sammine-lang, this is helpful in
//! debugging

//! .
//! .
class Location {
public:
  size_t source_start; // True location in original source code string
  size_t source_end;

  // Default constructor
  Location() : source_start(0), source_end(0) {}

  Location(size_t source_start, size_t source_end)
      : source_start(source_start), source_end(source_end) {}

  // Advance column position
  inline void advance() { source_end++; }

  // Move column position backwards
  inline void devance() { source_end--; }

  // Handle newline
  inline void newLine() { advance(); }

  // Combine two locations (union of spans)
  Location operator|(const Location &other) const {
    Location result;
    result.source_start = std::min(source_start, other.source_start);
    result.source_end = std::max(source_end, other.source_end);
    return result;
  }

  void operator|=(const Location &other) {
    source_start = std::min(source_start, other.source_start);
    source_end = std::max(source_end, other.source_end);
  }
  operator std::pair<size_t, size_t>() const {
    return std::make_pair(source_start, source_end);
  }

  // Stream output operator
  friend std::ostream &operator<<(std::ostream &out, const Location &loc) {
    out << loc.source_start << ":" << loc.source_end;

    return out;
  }

  std::string to_string() {
    return fmt::format("{}:{}", source_start, source_end);
  }

  // Equality operator
  bool operator==(const Location &other) const {
    return source_start == other.source_start && source_end == other.source_end;
  }
};
class Reportee {
public:
  enum ReportKind {
    error,
    warn,
    diag,
  };
  using Report = std::tuple<Location, std::string, ReportKind>;

  using iterator = std::vector<Report>::iterator;
  using const_iterator = std::vector<Report>::const_iterator;

  // Iterator methods
  iterator begin() { return reports.begin(); }
  iterator end() { return reports.end(); }
  const_iterator begin() const { return reports.begin(); }
  const_iterator end() const { return reports.end(); }
  const_iterator cbegin() const { return reports.cbegin(); }
  const_iterator cend() const { return reports.cend(); }

  void add_error(Location loc, std::string msg) {
    reports.push_back({loc, msg, ReportKind::error});
    _has_error = true;
  }
  void add_warn(Location loc, std::string msg) {
    reports.push_back({loc, msg, ReportKind::warn});
    _has_warn = true;
  }
  void add_diagnostics(Location loc, std::string msg) {
    reports.push_back({loc, msg, ReportKind::diag});
    _has_diag = true;
  }

  [[nodiscard]]
  bool has_errors() const {
    return this->_has_error;
  }
  [[nodiscard]]
  bool has_no_errors() const {
    return !this->_has_error;
  }
  [[nodiscard]]
  bool has_warn() const {
    return this->_has_warn;
  }
  [[nodiscard]]
  bool has_message() const {
    return !reports.empty();
  }

  [[nodiscard]]
  bool has_diagnostics() const {
    return this->_has_diag;
  }

private:
  std::vector<Report> reports;
  bool _has_error = false;
  bool _has_warn = false;
  bool _has_diag = false;
};

class Reporter {
  using ReportKind = Reportee::ReportKind;
  using IndexPair = std::pair<size_t, size_t>;
  static std::vector<std::pair<std::size_t, std::string_view>>
  get_diagnostic_data(std::string_view str) {
    decltype(get_diagnostic_data(str)) result;
    size_t start = 0;

    while (start < str.size()) {
      size_t end = str.find('\n', start);

      if (end == std::string_view::npos) {
        // Add the last substring if no more newlines are found
        result.push_back({start, str.substr(start)});
        break;
      }

      // Add the substring excluding the newline character
      result.push_back({start, str.substr(start, end - start)});
      start = end + 1;
      assert(start != 0);
    }

    return result;
  }
  inline static fmt::terminal_color LINE_COLOR =
      fmt::terminal_color::bright_magenta;
  std::string file_name;
  std::string input;
  std::vector<std::pair<std::size_t, std::string_view>> diagnostic_data;
  size_t depth;
  fmt::terminal_color get_color_from(ReportKind report_kind) const;

  // INFO: Given the source start and source end, get the line index of them.
  IndexPair get_lines_indices(IndexPair) const;

  // INFO: Given the line index start and end, get the line index of them,
  // modified to accompany for depth.
  IndexPair
  get_lines_indices_with_depth(std::pair<size_t, size_t> index_pair) const;

  // INFO: Given the source start and source end and knowing that they fit on a
  // singular line, recalibrate source start and end so that they start indexing
  // from 0 at the singular line.
  std::tuple<size_t, size_t, size_t>
      get_start_end_of_singular_line_token(IndexPair) const;

  void report(std::pair<size_t, size_t> index_pair,
              const std::string &format_str,
              const ReportKind report_kind) const;

  template <typename... T>
  void report(fmt::terminal_color ts, fmt::format_string<T...> format_str,
              T &&...args) const {
    fmt::print(stderr, fg(LINE_COLOR), format_str,
               std::forward<T>(args)...); // Print green color code
  }
  template <typename... T>
  void report(const ReportKind report_kind, fmt::format_string<T...> format_str,
              T &&...args) const {
    fmt::print(stderr, fg(get_color_from(report_kind)), format_str,
               std::forward<T>(args)...); // Print green color code
  }

public:
  void report_and_abort(const Reportee &reports) const;
  Reporter() {}
  Reporter(std::string file_name, std::string input, size_t depth)
      : file_name(file_name), input(input),
        diagnostic_data(get_diagnostic_data(this->input)), depth(depth) {
    /*size_t i = 0;*/
    /*for (auto [a, b] : diagnostic_data) {*/
    /*  std::cout << i++ << " " << a << " " << b << std::endl;*/
    /*}*/
  }
};
} // namespace sammine_util
