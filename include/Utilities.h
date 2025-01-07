#pragma once
#include "Lexer.h"
#include "fmt/color.h"
#include "fmt/core.h"
#include <__algorithm/ranges_lower_bound.h>
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

namespace sammine_util {
auto get_string_from_file(std::string file_name) -> std::string;
inline size_t unique_ast_id = 0;
auto abort(const std::string &message = "<NO MESSAGE>") -> void;
auto abort_on(bool abort_if_true, const std::string &message = "<NO MESSAGE>")
    -> void;

class Reports {
public:
  enum ReportKind {
    error,
    warn,
  };
  using Report = std::tuple<sammine_lang::Location, std::string, ReportKind>;

  using iterator = std::vector<Report>::iterator;
  using const_iterator = std::vector<Report>::const_iterator;

  // Iterator methods
  iterator begin() { return reports.begin(); }
  iterator end() { return reports.end(); }
  const_iterator begin() const { return reports.begin(); }
  const_iterator end() const { return reports.end(); }
  const_iterator cbegin() const { return reports.cbegin(); }
  const_iterator cend() const { return reports.cend(); }

  void add_error(sammine_lang::Location loc, std::string msg) {
    reports.push_back({loc, msg, ReportKind::error});
    _has_error = true;
  }
  void add_warn(sammine_lang::Location loc, std::string msg) {
    reports.push_back({loc, msg, ReportKind::warn});
    _has_warn = true;
  }
  void add_diagnostics(sammine_lang::Location loc, std::string msg) {
    reports.push_back({loc, msg, ReportKind::warn});
    _has_warn = true;
  }

  bool has_error() const { return this->_has_error; }
  bool has_warn() const { return this->_has_warn; }

private:
  std::vector<Report> reports;
  bool _has_error;
  bool _has_warn;
};
class Reporter {

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
    }

    return result;
  }
  inline static fmt::terminal_color LINE_COLOR =
      fmt::terminal_color::bright_magenta;
  std::string input;
  std::vector<std::pair<std::size_t, std::string_view>> diagnostic_data;

  fmt::terminal_color get_color_from(Reports::ReportKind report_kind) const {
    switch (report_kind) {
    case Reports::error:
      return fmt::terminal_color::bright_red;
      break;
    case Reports::warn:
      return fmt::terminal_color::bright_yellow;
      break;
    }
  }

public:
  void report_and_abort(const Reports &reports, size_t depth) const {
    for (auto &[loc, report_msg, report_kind] : reports) {
      auto [start, end] = loc;

      auto get_line_ite = [this](size_t source_index) -> size_t {
        auto cmp = [](const auto &a, const auto &b) {
          return a.first < b.first;
        };

        return std::next(std::ranges::lower_bound(
                   diagnostic_data,
                   std::make_pair(source_index, std::string_view("")), cmp)) -
               diagnostic_data.begin();
      };
      auto line_start = get_line_ite(start), line_end = get_line_ite(end);
      line_start = line_start > depth ? line_start - depth : 0;
      line_end = line_end + depth <= diagnostic_data.size() - 1
                     ? line_end + depth
                     : diagnostic_data.size() - 1;

      /*fmt::println("Start: {}, end : {}", line_start, line_end);*/
      fmt::print(stderr, fg(get_color_from(report_kind)), "-----{}\n",
                 report_msg);
      for (auto i = line_start; i <= line_end; i++) {
        fmt::print(stderr, fg(LINE_COLOR), "{}:", i + 1);
        fmt::print(stderr, fg(get_color_from(report_kind)), "{}\n",
                   diagnostic_data[i].second);
      }

      fmt::println("");
    }

    if (reports.has_error() || reports.has_warn()) {

      fmt::print(stderr, fg(LINE_COLOR),
                 "\nDid something seems wrong? Report it via "
                 "[XXYYZZ]\n");
    }
    if (reports.has_error())
      std::exit(1);
  }
  Reporter() {}
  Reporter(std::string input)
      : input(input), diagnostic_data(get_diagnostic_data(this->input)) {}
};
} // namespace sammine_util
