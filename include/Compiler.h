//
// Created by Jasmine Tang on 3/27/24.
//

#pragma once

#include "LLVMRes.h"
#include "Lexer.h"
#include "Parser.h"
#include "Utilities.h"
#include <memory>
#include <string>
namespace sammine_lang {
enum class compiler_option_enum {
  FILE,
  STR,
  LLVM_IR,
  DIAGNOSTIC,
};
class Compiler {
  std::shared_ptr<TokenStream> tokStream;
  std::shared_ptr<AST::ProgramAST> programAST;
  std::map<compiler_option_enum, std::string> compiler_options;
  std::shared_ptr<LLVMRes> resPtr;
  std::string file_name, input;
  sammine_util::Reporter reporter;
  size_t context_radius = 2;
  bool error;

  void lex();
  void parse();
  void scopecheck();
  void typecheck();
  void codegen();
  void produce_executable();
  void set_error() { error = true; }
  void log_diagnostics(const std::string &diagnostics);
  void force_log_diagnostics(const std::string &diagnostics);

public:
  Compiler(std::map<compiler_option_enum, std::string> &compiler_options);
  void start();
};
} // namespace sammine_lang
