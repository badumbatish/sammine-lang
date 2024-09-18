#include "Utilities.h"
#include "fmt/color.h"
#include "fmt/core.h"
#include "fmt/format.h"
#include <cpptrace/cpptrace.hpp>
#include <cpptrace/from_current.hpp>
#include <cstdlib>
namespace sammine_util {
auto get_string_from_file(std::string file_name) -> std::string {
  auto file = FileRAII(file_name);

  std::string error_msg =
      fmt::format("Cannot find or open file : {}", file_name);
  abort_on(!file.isOpen(), error_msg);
  auto input = file.getInternalStr();

  return input;
}

auto abort(const std::string &message) -> void {
  fmt::print(stderr, fg(fmt::color::red), "[Internal Compiler Error] : {}\n",
             message);
  fmt::print(stderr, fg(fmt::color::red), "[Generating stack traces]...\n");
  auto trace = cpptrace::generate_trace();
  trace.print_with_snippets();
  std::abort();
}

auto abort_on(bool abort_if_true, const std::string &message) -> void {
  if (abort_if_true) {
    abort(message);
  }
}
size_t get_unique_ast_id() { return unique_ast_id; }
} // namespace sammine_util
