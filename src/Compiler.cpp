//
// Created by Jasmine Tang on 3/27/24.
//

#include "Compiler.h"
#include "CodegenVisitor.h"
#include "Utilities.h"
#include "fmt/color.h"
#include "fmt/core.h"
#include "fmt/format.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/CodeGen.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"
#include <system_error>
namespace sammine_lang {

Compiler::Compiler(
    std::map<compiler_option_enum, std::string> &compiler_options)
    : compiler_options(compiler_options) {
  this->error = false;
  this->file_name = compiler_options[compiler_option_enum::FILE];
  this->input = compiler_options[compiler_option_enum::STR];
  if (this->input != "") {
    this->file_name = "From string input";
  } else if (this->file_name != "") {
    this->input = sammine_util::get_string_from_file(this->file_name);
  } else {
    fmt::print(stderr, fg(fmt::color::red),
               "[Error during compiler initial phase]\n");
    fmt::print(stderr, fg(fmt::color::red),
               "[Both the file name and the string input is empty]\n");
    set_error();
  }
  this->resPtr = std::make_shared<LLVMRes>();
}
void Compiler::lex() {

  Lexer lxr = Lexer(input);
  if (lxr.getTokenStream()->hasErrors()) {
    set_error();
    fmt::print(stderr, fg(fmt::color::red), "[Error during lexing phase]\n");
    auto stream = lxr.getTokenStream();
    for (auto i : stream->ErrStream) {

      fmt::print(stderr, "{}:{}: Encountered invalid token : {}\n", file_name,
                 i->location.to_string(), i->lexeme);
    }
  }

  tokStream = lxr.getTokenStream();
}

void Compiler::parse() {
  Parser psr = Parser(tokStream);

  auto result = psr.Parse();
  if (result.has_value()) {
    programAST = std::move(result.value());
  } else if (psr.hasErrors()) {
    set_error();
    fmt::print(stderr, "[Error during parsing phase]\n");
    for (auto i : psr.error_msgs) {
      fmt::print(stderr, "{}: {}\n", file_name, i);
    }
  }
}

void Compiler::codegen() {
  std::cerr << "Start partial codegen" << std::endl;
  auto cg = std::make_shared<sammine_lang::AST::CgVisitor>(resPtr);
  assert(cg != nullptr);
  programAST->accept_vis(cg.get());
  std::cerr << programAST->DefinitionVec.size() << std::endl;

  std::cerr << "Finish partial codegen" << std::endl;
  // TODO : Check for codegen error
  //
}

void Compiler::produce_executable() {

  std::cerr << "Starting .o" << std::endl;
  resPtr->Module->print(llvm::errs(), nullptr);
  llvm::raw_fd_ostream dest(llvm::raw_fd_ostream(resPtr->FileName, resPtr->EC));
  if (resPtr->EC) {
    llvm::errs() << "Could not open file: " << resPtr->EC.message();
    return;
  }

  auto FileType = llvm::CodeGenFileType::ObjectFile;

  if (resPtr->target_machine->addPassesToEmitFile(resPtr->pass, dest, nullptr,
                                                  FileType)) {
    llvm::errs() << "TargetMachine can't emit a file of this type";
    return;
  }

  resPtr->pass.run(*resPtr->Module);
  dest.flush();

  set_error();
  std::cerr << "ending .o " << std::endl;
}
void Compiler::start() {
  using CompilerStage = std::function<void(Compiler *)>;
  std::vector<std::pair<CompilerStage, std::string>> CompilerStages = {
      {&Compiler::lex, "lexing"},
      {&Compiler::parse, "parsing"},
      {&Compiler::codegen, "codegen"},
      {&Compiler::produce_executable, "produce_executable"},
  };

  std::string prev = "";
  for (auto stage : CompilerStages) {
    if (!error) {
      stage.first(this);
      prev = stage.second;
    } else {
      std::cerr << std::endl
                << "Sammine-lang compiler done processing" << std::endl;
      break;
    }
  }
}

} // namespace sammine_lang
