#include "Utilities.h"
#include "FileRAII.h"
#include "fmt/color.h"
#include "fmt/core.h"
#include <cassert>
#include <cctype>
#include <cpptrace/cpptrace.hpp>
#include <cpptrace/from_current.hpp>
#include <cstdlib>
namespace sammine_util {
auto get_string_from_file(const std::string &file_name) -> std::string {
  auto file = FileRAII(file_name);

  std::string error_msg =
      fmt::format("Cannot find or open file : {}", file_name);
  abort_on(!file.isOpen(), error_msg);
  auto input = file.getInternalStr();

  return input;
}
inline auto abort(const std::string &message) -> void {
  fmt::print(stderr, fg(fmt::terminal_color::bright_red),
             "[Internal Compiler Error] : {}\n", message);
  fmt::print(stderr, fg(fmt::terminal_color::bright_red),
             "[Generating stack traces]...\n");
  fmt::print(stderr, fg(fmt::terminal_color::bright_red), "[Please wait]...\n");
  auto trace = cpptrace::generate_trace();
  trace.print_with_snippets();
  std::abort();
}

inline auto abort_on(bool abort_if_true, const std::string &message) -> void {
  if (abort_if_true) {
    abort(message);
    std::abort();
  }
}
Reporter::IndexPair Reporter::get_lines_indices(IndexPair index_pair) const {
  auto [start, end] = index_pair;
  // INFO: Helper function
  auto get_line_ite = [this](size_t source_index) -> size_t {
    auto cmp = [](const auto &a, const auto &b) { return a.first < b.first; };

    auto idx = std::ranges::lower_bound(
                   diagnostic_data,
                   std::make_pair(source_index, std::string_view("")), cmp) -
               diagnostic_data.begin();
    return idx + (idx == 0);
  };

  return {get_line_ite(start) - 1, get_line_ite(end) - 1};
}

Reporter::IndexPair
Reporter::get_lines_indices_with_depth(IndexPair index_pair) const {
  auto [line_start, line_end] = index_pair;
  line_start = line_start > depth ? line_start - depth : 0;
  line_end = line_end + depth <= diagnostic_data.size() - 1
                 ? line_end + depth
                 : diagnostic_data.size() - 1;

  return {line_start, line_end};
}

std::tuple<size_t, size_t, size_t>
Reporter::get_start_end_of_singular_line_token(
    Reporter::IndexPair index_pair) const {

  auto [start, end] = index_pair;
  // INFO: Helper function
  auto get_line_ite = [this](size_t source_index) -> size_t {
    auto cmp = [](const auto &a, const auto &b) { return a.first < b.first; };

    auto idx = std::ranges::lower_bound(
                   diagnostic_data,
                   std::make_pair(source_index, std::string_view("")), cmp) -
               diagnostic_data.begin();
    return idx + (idx == 0);
  };

  auto num_row = get_line_ite(start) - 1;
  auto num_col = start - diagnostic_data[num_row].first;

  return {num_row, num_col, num_col + end - start};
}

void Reporter::report(std::pair<size_t, size_t> index_pair,
                      const std::string &format_str,
                      const ReportKind report_kind) const {

  auto [og_start, og_end] = get_lines_indices(index_pair);
  auto [new_start, new_end] = get_lines_indices_with_depth({og_start, og_end});
  auto [row_num, col_start, col_end] =
      get_start_end_of_singular_line_token(index_pair);

  bool same_line = og_start == og_end;
  report(LINE_COLOR, "    |");
  auto true_row = row_num + 1;
  report(report_kind, "At {}:{}:{}\n", file_name, true_row, col_start);
  report(LINE_COLOR, "    |");
  report(report_kind, "{}\n", format_str);
  for (auto i = new_start; i <= new_end; i++) {
    true_row = i + 1;
    report(LINE_COLOR, "{:>4}|", true_row);
    report(report_kind, "{}\n", diagnostic_data[i].second);

    if (same_line && i == og_start) {
      report(LINE_COLOR, "    |");

      bool start_printing = false;
      for (auto ch : diagnostic_data[i].second) {
        start_printing = start_printing || !isspace(ch);
        if (!start_printing)
          report(report_kind, "{}", ch);
        else
          report(report_kind, "~");
      }

      report(report_kind, "\n");
    }
  }
}

void Reporter::report_and_abort(const Reportee &reports) const {

  bool begin = true;
  for (const auto &[loc, report_msg, report_kind] : reports) {

    for (size_t i = 1; i <= 2 && !begin; i++)
      report(LINE_COLOR, "    |\n");

    begin = false;
    report(loc, report_msg, report_kind);
  }

  if (reports.has_message()) {
    report(LINE_COLOR,
           "\n# Did something seems wrong? Report it via "
           "[https://github.com/badumbatish/sammine-lang/issues]\n");
    report(LINE_COLOR,
           "# Give us a screenshot of the error as well as your source code "
           "surrounding the error\n");
  }
  if (reports.has_errors())
    std::exit(1);
}
fmt::terminal_color Reporter::get_color_from(ReportKind report_kind) const {
  switch (report_kind) {
  case Reportee::error:
    return fmt::terminal_color::bright_red;
    break;
  case Reportee::warn:
    return fmt::terminal_color::bright_yellow;
    break;
  case Reportee::diag:
    return fmt::terminal_color::bright_green;
    break;
  }
}

size_t get_unique_ast_id() { return unique_ast_id; }
} // namespace sammine_util
