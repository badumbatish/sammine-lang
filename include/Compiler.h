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
  public:
    Compiler(std::string input, std::string file_name) {
      Lexer lxr = Lexer(input);


      if (lxr.getTokenStream()->hasErrors()) {
        std::cerr << "Error during lexing phase " << std::endl;
        auto tokStream = lxr.getTokenStream();
        for (auto i : tokStream->ErrStream) {
          std::cerr << "Encountered invalid : " << i->lexeme << " at " << i->location << std::endl;
        }
      }
    }
  };
}
#endif // SAMMINE_LANG_COMPILER_H
