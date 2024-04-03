//
// Created by Jasmine Tang on 3/27/24.
//

#include "Compiler.h"

namespace sammine_lang {

Compiler::Compiler(const std::string &input, const std::string &file_name)
    : input(input), file_name(file_name), error(false) {}
void Compiler::lex() {

  Lexer lxr = Lexer(input);
  if (lxr.getTokenStream()->hasErrors()) {
    set_error();
    std::cerr << "[Error during lexing phase]" << std::endl;
    auto stream = lxr.getTokenStream();
    for (auto i : stream->ErrStream) {
      std::cerr << file_name << i->location <<  ": Encountered invalid : " << i->lexeme << std::endl;
    }
  }

  tokStream = lxr.getTokenStream();
}

void Compiler::parse() {
  Parser psr = Parser(tokStream);

  programAST = psr.Parse();

  if (psr.hasErrors()) {
    set_error();
    std::cerr << "[Error during parsing phase]" << std::endl;
    for (auto i : psr.error_msgs) {
      std::cerr << file_name << ":" << i << std::endl;
    }
  }
}

void Compiler::start() {
  using CompilerStage = std::function<void(Compiler *)>;
  std::vector<CompilerStage> CompilerStages = {&Compiler::lex,
                                               &Compiler::parse};

  for (auto stage : CompilerStages) {
    if (!error) {
      stage(this);
    } else {
      break;
    }
  }
}

} // namespace sammine_lang