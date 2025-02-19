//
// Created by Jasmine Tang on 3/27/24.
//

#include "Compiler.h"
#include "CodegenVisitor.h"
#include "ScopeGeneratorVisitor.h"
#include "Utilities.h"
#include "fmt/color.h"
#include "fmt/core.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/CodeGen.h"
#include "llvm/Support/raw_ostream.h"
#include <memory>
#include <system_error>
namespace sammine_lang {

void Compiler::log_diagnostics(const std::string &diagnostics) {
  if (compiler_options[compiler_option_enum::DIAGNOSTIC] == "true")
    Compiler::force_log_diagnostics(diagnostics);
}
inline void Compiler::force_log_diagnostics(const std::string &diagnostics) {
  fmt::print(stderr, fg(fmt::terminal_color::bright_green), "{}\n",
             diagnostics);
}
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
    fmt::print(stderr, fg(fmt::terminal_color::bright_red),
               "[Error during compiler initial phase]\n");
    fmt::print(stderr, fg(fmt::terminal_color::bright_red),
               "[Both the file name and the string input is empty]\n");

    std::abort();
  }
  this->resPtr = std::make_shared<LLVMRes>();

  this->reporter = sammine_util::Reporter(file_name, input, report_width);
}

void Compiler::lex() {
  log_diagnostics("Start lexing stage...");
  Lexer lxr = Lexer(input);
  reporter.report_and_abort(lxr);
  tokStream = lxr.getTokenStream();
}

void Compiler::parse() {
  log_diagnostics("Start parser stage...");
  Parser psr = Parser(tokStream);

  auto result = psr.Parse();
  if (result.has_value())
    programAST = std::move(result.value());

  reporter.report_and_abort(psr);
}

void Compiler::scopecheck() {
  log_diagnostics("Start scope checking stage...");
  auto sc = sammine_lang::AST::ScopeGeneratorVisitor();

  programAST->accept_vis(&sc);
  reporter.report_and_abort(sc);
}
void Compiler::codegen() {
  log_diagnostics("Start codegen stage...");
  auto cg = sammine_lang::AST::CgVisitor(resPtr);
  programAST->accept_vis(&cg);

  // TODO : Check for codegen error
  //
}

void Compiler::produce_executable() {

  log_diagnostics("Start producing executable/object file stage");
  if (compiler_options[compiler_option_enum::LLVM_IR] == "true") {
    force_log_diagnostics("Logging pre optimization llvm IR");
    resPtr->Module->print(llvm::errs(), nullptr);
  }
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

  if (compiler_options[compiler_option_enum::LLVM_IR] == "true") {
    force_log_diagnostics("Logging post optimization llvm IR");
    resPtr->Module->print(llvm::errs(), nullptr);
  }
  set_error();
}
void Compiler::start() {
  using CompilerStage = std::function<void(Compiler *)>;
  std::vector<std::pair<CompilerStage, std::string>> CompilerStages = {
      {&Compiler::lex, "lexing"},
      {&Compiler::parse, "parsing"},
      {&Compiler::scopecheck, "scope check"},
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
