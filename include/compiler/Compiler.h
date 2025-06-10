//
// Created by Jasmine Tang on 3/27/24.
//

#pragma once
#include <map>
#include <string>
namespace sammine_lang {
enum compiler_option_enum {
  FILE,
  STR,
  LLVM_IR,
  AST_IR,
  DIAGNOSTIC,
  CHECK,
};

class CompilerRunner {
public:
  static void
  run(std::map<compiler_option_enum, std::string> &compiler_options);
};
} // namespace sammine_lang
