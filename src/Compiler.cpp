//
// Created by Jasmine Tang on 3/27/24.
//

#include "Compiler.h"

namespace sammine_lang {
  Compiler::Compiler(const std::string &input, const std::string &file_name)
    :
      input(input),
      file_name(file_name) {}
  void Compiler::lex() {
    Lexer lxr = Lexer(input);

    if (lxr.getTokenStream()->hasErrors()) {
      std::cerr << "Error during lexing phase " << std::endl;
      auto tokStream = lxr.getTokenStream();
      for (auto i : tokStream->ErrStream) {
        std::cerr << "Encountered invalid : " << i->lexeme << " at " << i->location << std::endl;
      }
    }

    tokStream = lxr.getTokenStream();
  }

  void Compiler::parse() {
    Parser psr = Parser(tokStream);

    programAST = psr.Parse();
  }
  void Compiler::start() {
    lex();
    parse();
  }
}