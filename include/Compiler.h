//
// Created by Jasmine Tang on 3/27/24.
//

#ifndef SAMMINE_LANG_COMPILER_H
#define SAMMINE_LANG_COMPILER_H

#include "Lexer.h"
#include "Parser.h"
#include <string>
namespace sammine_lang {
class Compiler {
  std::shared_ptr<TokenStream> tokStream;
  std::unique_ptr<AST::ProgramAST> programAST;
  std::string input, file_name;
  bool error;

  void lex();
  void parse();
  void codegen();
  void set_error() { error = true; }

public:
  Compiler(const std::string &input, const std::string &file_name);
  void start();
};
} // namespace sammine_lang
#endif // SAMMINE_LANG_COMPILER_H
