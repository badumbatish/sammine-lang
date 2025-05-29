//
// Created by Jasmine Tang on 3/27/24.
//

#include "Compiler.h"
#include "Ast.h"
#include "BiTypeChecker.h"
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

  *this->resPtr;
  this->reporter = sammine_util::Reporter(file_name, input, context_radius);
}

void Compiler::lex() {
  log_diagnostics(fmt::format("Start lexing stage..."));
  Lexer lxr = Lexer(input);
  reporter.report(lxr);
  tokStream = lxr.getTokenStream();
}

void Compiler::parse() {
  log_diagnostics(fmt::format("Start parsing stage..."));
  Parser psr = Parser(
      tokStream,
      this->compiler_options[compiler_option_enum::PARSE_DIAG] == "true");

  auto result = psr.Parse();
  if (result.has_value())
    programAST = std::move(result.value());

  reporter.report(psr);

  this->error = psr.has_errors();
}

void Compiler::scopecheck() {
  if (this->error) {
    return;
  }
  log_diagnostics(fmt::format("Start scope checking stage..."));
  auto vs = sammine_lang::AST::ScopeGeneratorVisitor();

  programAST->accept_vis(&vs);
  reporter.report(vs);
  this->error = vs.has_errors();
}

void Compiler::typecheck() {
  if (this->error) {
    return;
  }
  log_diagnostics(fmt::format("Start bi-direcitonal type checking stage..."));
  auto vs = sammine_lang::AST::BiTypeCheckerVisitor();
  programAST->accept_vis(&vs);
  reporter.report(vs);
  this->error = vs.has_errors();
}

void Compiler::dump_ast() {
  if (compiler_options[compiler_option_enum::AST_IR] == "true") {
    log_diagnostics(fmt::format("Start dumping ast-ir stage..."));
    AST::ASTPrinter::print(programAST.get());
  }
}
void Compiler::codegen() {
  if (this->error) {
    std::exit(1);
  }
  log_diagnostics(fmt::format("Start code-gen stage..."));
  auto vs = sammine_lang::AST::CgVisitor(resPtr);
  programAST->accept_vis(&vs);

  reporter.report(vs);
  this->error = vs.has_errors();
}

void Compiler::produce_executable() {
  if (this->error) {
    std::exit(1);
  }

  log_diagnostics(fmt::format("Start executable/lib stage..."));
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
}
void Compiler::start() {
  using CompilerStage = std::function<void(Compiler *)>;
  std::vector<CompilerStage> CompilerStages = {
      {&Compiler::lex},
      {&Compiler::parse},
      {&Compiler::scopecheck},
      {&Compiler::typecheck},
      {&Compiler::dump_ast},
      {&Compiler::codegen},
      {&Compiler::produce_executable},
  };

  // no error, proceed with current stage
  // error, skip current stage and go next
  // error, compiler-ending stage
  std::string prev = "";
  for (auto stage : CompilerStages) {
    stage(this);
  }
}

} // namespace sammine_lang
