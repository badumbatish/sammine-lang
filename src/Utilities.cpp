#include "Utilities.h"
#include "FileRAII.h"
#include "fmt/color.h"
#include "fmt/core.h"
#include <cassert>
#include <cctype>
#include <cpptrace/cpptrace.hpp>
#include <cpptrace/from_current.hpp>
#include <cstdlib>
#include <string_view>
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

namespace {
using IndexPair = Reporter::IndexPair;
using DiagnosticData = Reporter::DiagnosticData;
class Locator {
  IndexPair source_start_end;
  size_t context_radius;
  const Reporter::DiagnosticData &data;

public:
  Locator(IndexPair source_start_end, size_t context_radius,
          const Reporter::DiagnosticData &data)
      : source_start_end(source_start_end), context_radius(context_radius),
        data(data) {}

  // INFO: Given the source start and source end, get the line index of them.
  IndexPair get_lines_indices() const {
    auto [start, end] = source_start_end;
    // INFO: Helper function
    auto get_line_ite = [this](size_t source_index) -> size_t {
      auto cmp = [](const auto &a, const auto &b) { return a.first < b.first; };

      auto idx =
          std::ranges::lower_bound(
              data, std::make_pair(source_index, std::string_view("")), cmp) -
          data.begin();
      return idx + (idx == 0);
    };

    return {get_line_ite(start) - 1, get_line_ite(end) - 1};
  }

  // INFO: Given the line index start and end, get the line index of them,
  // modified to accompany for context radius.
  IndexPair get_lines_indices_with_radius() const {
    auto [line_start, line_end] = get_lines_indices();
    line_start = line_start > context_radius ? line_start - context_radius : 0;
    line_end = line_end + context_radius <= data.size() - 1
                   ? line_end + context_radius
                   : data.size() - 1;

    return {line_start, line_end};
  }

  // INFO: Given the source start and source end and knowing that they fit on a
  // singular line, recalibrate source start and end so that they start indexing
  // from 0 at the singular line.
  std::tuple<size_t, size_t, size_t>
  get_start_end_of_singular_line_token() const {

    auto [start, end] = source_start_end;
    // INFO: Helper function
    auto get_line_ite = [this](size_t source_index) -> size_t {
      auto cmp = [](const auto &a, const auto &b) { return a.first < b.first; };

      auto idx =
          std::ranges::lower_bound(
              data, std::make_pair(source_index, std::string_view("")), cmp) -
          data.begin();
      return idx + (idx == 0);
    };

    auto num_row = get_line_ite(start) - 1;
    auto num_col = start - data[num_row].first;

    return {num_row, num_col, num_col + end - start};
  }

  IndexPair get_row_col() const {
    auto [start, end] = source_start_end;
    // INFO: Helper function
    auto get_line_ite = [this](size_t source_index) -> size_t {
      auto cmp = [](const auto &a, const auto &b) { return a.first < b.first; };

      auto idx =
          std::ranges::lower_bound(
              data, std::make_pair(source_index, std::string_view("")), cmp) -
          data.begin();
      return idx + (idx == 0);
    };

    auto num_row = get_line_ite(start) - 1;
    auto num_col = start - data[num_row].first;

    return {num_row, num_col};
  }

  bool is_on_singular_line(size_t i) {
    auto [og_start, og_end] = this->get_lines_indices();
    return og_start == og_end && i == og_start;
  }
  bool is_on_singular_line() {
    auto [og_start, og_end] = this->get_lines_indices();
    return og_start == og_end;
  }
};
} // namespace

DiagnosticData Reporter::get_diagnostic_data(std::string_view str) {
  DiagnosticData result;
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
void Reporter::indicate_singular_line(ReportKind report_kind, size_t col_start,
                                      size_t col_end) const {

  report(LINE_COLOR, "    |");
  size_t j = 0;
  for (; j < col_start; j++)
    report(report_kind, " ");

  for (; j < col_end; j++)
    report(report_kind, "^");
  report(report_kind, "\n");
}

void Reporter::report_singular_line(ReportKind report_kind,
                                    const std::string &msg, size_t col_start,
                                    size_t col_end) const {
  report(LINE_COLOR, "    |");
  size_t j = 0;
  for (; j < col_start; j++)
    report(report_kind, " ");

  report(report_kind, "{}", msg);
  report(report_kind, "\n");
}
void Reporter::report(std::pair<size_t, size_t> index_pair,
                      const std::string &format_str,
                      const ReportKind report_kind) const {
  Locator locator(index_pair, context_radius, diagnostic_data);
  auto [new_start, new_end] = locator.get_lines_indices_with_radius();
  auto [row_num, col_start, col_end] =
      locator.get_start_end_of_singular_line_token();

  report(LINE_COLOR, "    |");
  report(fmt::terminal_color::bright_blue, "At {}:{}:{}\n", file_name,
         row_num + 1, col_start);
  if (!locator.is_on_singular_line()) {
    report(LINE_COLOR, "    |");
    report(MSG_COLOR, "{}\n", format_str);
  }
  for (auto i = new_start; i <= new_end; i++) {
    report(LINE_COLOR, "{:>4}|", i + 1);
    std::string_view str = diagnostic_data[i].second;
    report(fmt::terminal_color::white, "{}\n", str);

    if (locator.is_on_singular_line(i)) {
      indicate_singular_line(report_kind, col_start, col_end);
      report_singular_line(report_kind, format_str, col_start, col_end);
    }
  }
}

void Reporter::report_and_abort(const Reportee &reports) const {

  bool begin = true;
  for (const auto &[loc, report_msg, report_kind] : reports) {

    for (size_t i = 1; i <= 1 && !begin; i++)
      report(LINE_COLOR, "----|\n");

    begin = false;
    report(loc, report_msg, report_kind);
  }

  if (reports.has_message()) {
    report(fmt::terminal_color::bright_green,
           "\n# Did something seems wrong? Report it via "
           "[https://github.com/badumbatish/sammine-lang/issues]\n");
    report(fmt::terminal_color::bright_green,
           "# Give us a screenshot of the error as well as your contextual "
           "source code\n");
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
