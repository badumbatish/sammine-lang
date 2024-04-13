//
// Created by Jasmine Tang on 3/27/24.
//

#include "Compiler.h"
#include "CodegenVisitor.h"
namespace sammine_lang {

Compiler::Compiler(const std::string &input, const std::string &file_name)
    : input(input), file_name(file_name), error(false) {
    resPtr = std::shared_ptr<LLVMRes>();
}
void Compiler::lex() {

  Lexer lxr = Lexer(input);
  if (lxr.getTokenStream()->hasErrors()) {
    set_error();
    std::cerr << "[Error during lexing phase]" << std::endl;
    auto stream = lxr.getTokenStream();
    for (auto i : stream->ErrStream) {
      std::cerr << file_name << ":" << i->location
                << ": Encountered invalid : " << i->lexeme << std::endl;
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

void Compiler::codegen() {
    std::cout << "Start partial codegen" << std::endl;
    std::unique_ptr<sammine_lang::AST::CgVisitor> cg =
      std::make_unique<sammine_lang::AST::CgVisitor>(resPtr);
    assert(cg != nullptr);
  programAST->accept_vis(cg.get());

  std::cout << "Finish partial codegen" << std::endl;
  // TODO : Check for codegen error
}

void Compiler::start() {
  using CompilerStage = std::function<void(Compiler *)>;
  std::vector<std::pair<CompilerStage, std::string>> CompilerStages = {
      {&Compiler::lex, "lexing"}, {&Compiler::parse, "parsing"},
      {&Compiler::codegen, "codegen"}};

  for (auto stage : CompilerStages) {
    if (!error) {
      stage.first(this);
    } else {
      std::cout << "Failed at " << stage.second << std::endl;
      break;
    }
  }
}

} // namespace sammine_lang