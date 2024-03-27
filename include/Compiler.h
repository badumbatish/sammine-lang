//
// Created by Jasmine Tang on 3/27/24.
//

#ifndef SAMMINE_LANG_COMPILER_H
#define SAMMINE_LANG_COMPILER_H

#include <string>
#include "Lexer.h"
#include "Parser.h"
namespace sammine_lang {
  class Compiler {
    std::shared_ptr<TokenStream> tokStream;
    std::unique_ptr<AST::ProgramAST> programAST;
    std::string input, file_name;
    bool error;

    void lex();
    void parse();

  public:
    Compiler(const std::string &input, const std::string &file_name);
    void start();
  };
}
#endif // SAMMINE_LANG_COMPILER_H
