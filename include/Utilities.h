#pragma once
#include "Lexer.h"
#include <cassert>
#include <string>
#include <vector>

namespace sammine_util {
auto get_string_from_file(std::string file_name) -> std::string;
inline size_t unique_ast_id = 0;
auto abort(const std::string &message = "<NO MESSAGE>") -> void;
auto abort_on(bool abort_if_true, const std::string &message = "<NO MESSAGE>")
    -> void;

class ErrorMsgs {
  using ErrorMsg = std::pair<sammine_lang::Location, std::string>;

public:
  std::vector<ErrorMsg> errors;

  std::vector<std::string> stringify() {
    std::vector<std::string> string_errors;
    for (auto &error : errors) {
      string_errors.push_back(error.second);
    }

    return string_errors;
  }
};
} // namespace sammine_util
