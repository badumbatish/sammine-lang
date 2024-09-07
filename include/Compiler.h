//
// Created by Jasmine Tang on 3/27/24.
//

#ifndef SAMMINE_LANG_COMPILER_H
#define SAMMINE_LANG_COMPILER_H

#include "LLVMRes.h"
#include "Lexer.h"
#include "Parser.h"
#include <string>
namespace sammine_lang {
enum class compiler_option_enum {
  FILE,
  STR,
  LLVM_IR,
};
class Compiler {
  std::shared_ptr<TokenStream> tokStream;
  std::unique_ptr<AST::ProgramAST> programAST;
  std::map<compiler_option_enum, std::string> &compiler_options;
  std::shared_ptr<LLVMRes> resPtr;
  std::string file_name, input;
  bool error;

  void lex();
  void parse();
  void codegen();
  void set_error() { error = true; }

public:
  Compiler(std::map<compiler_option_enum, std::string> &compiler_options);
  void start();
};
} // namespace sammine_lang
#endif // SAMMINE_LANG_COMPILER_H
