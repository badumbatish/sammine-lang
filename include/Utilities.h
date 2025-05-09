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
#include <string_view>
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
  // True location in original source code string
  size_t source_start;
  size_t source_end;

  // Default constructor
  Location() : source_start(0), source_end(0) {}

  Location(size_t source_start, size_t source_end)
      : source_start(source_start), source_end(source_end) {}

  // Advance column position
  inline void advance() { source_end++; }

  // Move column position backwards
  inline void devance() {
    source_end--;
    assert(source_end >= source_start);
  }

  // Handle newline
  inline void newLine() { advance(); }

  // Combine two locations
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
    error_count++;
  }
  void add_warn(Location loc, std::string msg) {
    reports.push_back({loc, msg, ReportKind::warn});
    warn_count++;
  }
  void add_diagnostics(Location loc, std::string msg) {
    reports.push_back({loc, msg, ReportKind::diag});
    diag_count++;
  }

  [[nodiscard]]
  bool has_errors() const {
    return error_count > 0;
  }
  [[nodiscard]]
  bool has_no_errors() const {
    return error_count == 0;
  }
  [[nodiscard]]
  bool has_warn() const {
    return warn_count > 0;
  }
  [[nodiscard]]
  bool has_message() const {
    return !reports.empty();
  }

  [[nodiscard]]
  bool has_diagnostics() const {
    return diag_count > 0;
  }

  [[nodiscard]]
  size_t get_error_count() const {
    return error_count;
  }

  [[nodiscard]]
  size_t get_warn_count() const {
    return warn_count;
  }
  [[nodiscard]]
  size_t get_diagnostic_count() const {
    return diag_count;
  }

private:
  std::vector<Report> reports;
  size_t error_count = 0;
  size_t warn_count = 0;
  size_t diag_count = 0;
};

class Reporter {
public:
  using ReportKind = Reportee::ReportKind;
  using IndexPair = std::pair<size_t, size_t>;
  using DiagnosticData = std::vector<std::pair<std::size_t, std::string_view>>;

private:
  static DiagnosticData get_diagnostic_data(std::string_view str);
  inline static fmt::terminal_color LINE_COLOR =
      fmt::terminal_color::bright_magenta;

  inline static fmt::terminal_color MSG_COLOR =
      fmt::terminal_color::bright_blue;
  std::string file_name;
  std::string input;
  std::vector<std::pair<std::size_t, std::string_view>> diagnostic_data;
  size_t context_radius;
  static fmt::terminal_color get_color_from(ReportKind report_kind);

  void report_single_msg(std::pair<size_t, size_t> index_pair,
                         const std::string &format_str,
                         const ReportKind report_kind) const;

  template <typename... T>
  static void print_fmt(fmt::terminal_color ts,
                        fmt::format_string<T...> format_str, T &&...args) {
    fmt::print(stderr, fg(ts), format_str, std::forward<T>(args)...);
  }
  template <typename... T>
  static void print_fmt(fmt::color ts, fmt::format_string<T...> format_str,
                        T &&...args) {
    fmt::print(stderr, fg(ts), format_str, std::forward<T>(args)...);
  }
  template <typename... T>
  static void print_fmt(const ReportKind report_kind,
                        fmt::format_string<T...> format_str, T &&...args) {
    fmt::print(stderr, fg(get_color_from(report_kind)), format_str,
               std::forward<T>(args)...);
  }

  void indicate_singular_line(ReportKind report_kind, size_t col_start,
                              size_t col_end) const;

  void report_singular_line(ReportKind report_kind, const std::string &msg,
                            size_t col_start, size_t col_end) const;

  void print_data_singular_line(std::string_view msg, size_t col_start,
                                size_t col_end) const;

public:
  void report_and_abort(const Reportee &reports) const;
  Reporter() {}
  Reporter(std::string file_name, std::string input, size_t context_radius)
      : file_name(file_name), input(input),
        diagnostic_data(get_diagnostic_data(this->input)),
        context_radius(context_radius) {}
};
} // namespace sammine_util
