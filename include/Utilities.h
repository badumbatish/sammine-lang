#pragma once
#include "Lexer.h"
#include "fmt/color.h"
#include "fmt/core.h"
#include <cassert>
#include <iostream>
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

class Reporter {
  static std::vector<std::string_view> split_by_newline(std::string_view str) {
    std::vector<std::string_view> result;
    size_t start = 0;

    while (start < str.size()) {
      size_t end = str.find('\n', start);

      if (end == std::string_view::npos) {
        // Add the last substring if no more newlines are found
        result.push_back(str.substr(start));
        break;
      }

      // Add the substring excluding the newline character
      result.push_back(str.substr(start, end - start));
      start = end + 1;
    }

    return result;
  }
  static void print_msg(sammine_lang::Location location, const std::string &msg,
                        const std::string &input_code, size_t width,
                        fmt::terminal_color color) {
    std::cout << input_code << " " << input_code.size() << " "
              << location.source_start << std::endl;
    auto modified = input_code.substr(location.source_start);

    auto lines = split_by_newline(modified);
    size_t endline_index = input_code.find_first_of('\n');
    std::cout << input_code.size() << std::endl;
    std::cout << "Line start : " << location.line_start << std::endl;
    std::cout << "Line end: " << location.line_start << std::endl;
    std::cout << "end Line index: " << endline_index << std::endl;

    fmt::print(stderr, fg(color), "------ {}\n", msg);

    size_t i = location.line_start;
    for (auto &line : lines) {
      fmt::print(stderr, fg(color), "{}: {}\n", i++, line);
    }
  }

public:
  enum ReportKind {
    error,
    warn,
  };

  using Report = std::tuple<sammine_lang::Location, std::string, ReportKind>;
  std::vector<Report> reports;

  void add_error(sammine_lang::Location loc, std::string msg) {
    reports.push_back({loc, msg, ReportKind::error});
  }
  void add_warn(sammine_lang::Location loc, std::string msg) {
    reports.push_back({loc, msg, ReportKind::warn});
  }

  static void report_and_abort(std::vector<Report> reports,
                               const std::string &input_code) {
    for (auto [loc, msg, report_kind] : reports) {
      switch (report_kind) {
      case error:
        print_msg(loc, msg, input_code, 2, fmt::terminal_color::bright_red);
        break;
      case warn:
        print_msg(loc, msg, input_code, 2, fmt::terminal_color::bright_yellow);
        break;
      }
    }
  }
};
} // namespace sammine_util
