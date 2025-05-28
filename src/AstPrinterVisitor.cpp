#include "AstPrinterVisitor.h"
#include "Ast.h"
#include "fmt/base.h"
#include "fmt/format.h"
#include <cstdio>
namespace sammine_lang::AST {
std::string &AstPrinterVisitor::tabs() { return this->current_tabs; }

void AstPrinterVisitor::visit(PrototypeAST *ast) {
  ast->walk_with_preorder(this);
  ast->walk_with_postorder(this);
}
void AstPrinterVisitor::generic_preprintln(const std::string &tree_name) {
  this->rep += fmt::format("{} {} \n", tabs(), tree_name);
  current_tabs += "  ";
}
void AstPrinterVisitor::generic_preprint(const std::string &tree_name) {
  this->rep += fmt::format("{} {}: ", tabs(), tree_name);
  current_tabs += "  ";
}

void AstPrinterVisitor::generic_postprint() {
  current_tabs.pop_back();
  current_tabs.pop_back();
  this->rep += fmt::format("{}\n", tabs());
}
void AstPrinterVisitor::preorder_walk(ProgramAST *ast) {
  generic_preprintln(ast->getTreeName());
}
void AstPrinterVisitor::preorder_walk(VarDefAST *ast) {
  generic_preprintln(ast->getTreeName());
}
void AstPrinterVisitor::preorder_walk(ExternAST *ast) {
  generic_preprintln(ast->getTreeName());
}
void AstPrinterVisitor::preorder_walk(FuncDefAST *ast) {
  generic_preprintln(ast->getTreeName());
}
void AstPrinterVisitor::preorder_walk(PrototypeAST *ast) {
  generic_preprintln(ast->getTreeName());

  this->rep += fmt::format("{} fn_name: \"{}\"\n", tabs(), ast->functionName);
  this->rep += fmt::format("{} fn_sig: {} ", tabs(), ast->type.to_string());
}
void AstPrinterVisitor::preorder_walk(CallExprAST *ast) {
  generic_preprintln(ast->getTreeName());
}
void AstPrinterVisitor::preorder_walk(BinaryExprAST *ast) {
  generic_preprintln(ast->getTreeName());
}
void AstPrinterVisitor::preorder_walk(NumberExprAST *ast) {
  generic_preprint(ast->getTreeName());
  this->rep += fmt::format("(num, type): (\"{}\", {})", ast->number,
                           ast->type.to_string());
}
void AstPrinterVisitor::preorder_walk(BoolExprAST *ast) {
  generic_preprintln(ast->getTreeName());
}
void AstPrinterVisitor::preorder_walk(VariableExprAST *ast) {
  generic_preprintln(ast->getTreeName());
}
void AstPrinterVisitor::preorder_walk(BlockAST *ast) {
  generic_preprintln(ast->getTreeName());
}
void AstPrinterVisitor::preorder_walk(IfExprAST *ast) {
  generic_preprintln(ast->getTreeName());
}
void AstPrinterVisitor::preorder_walk(TypedVarAST *ast) {
  generic_preprint(ast->getTreeName());
  this->rep += fmt::format("(name, type_lxm, type): (\"{}\", \"{}\", {})",
                           ast->name, ast->type_lexeme, ast->type.to_string());
}

// post order
void AstPrinterVisitor::postorder_walk(ProgramAST *ast) {
  generic_postprint();
  while (!this->rep.empty()) {
    auto &ch = this->rep.back();
    if (ch == ' ' || ch == '\n') {
      this->rep.pop_back();
    } else {
      break;
    }
  }
  this->rep += "\n";

  fmt::print("{}", this->rep);
}
void AstPrinterVisitor::postorder_walk(VarDefAST *ast) { generic_postprint(); }
void AstPrinterVisitor::postorder_walk(ExternAST *ast) { generic_postprint(); }
void AstPrinterVisitor::postorder_walk(FuncDefAST *ast) { generic_postprint(); }
void AstPrinterVisitor::postorder_walk(PrototypeAST *ast) {
  generic_postprint();
}
void AstPrinterVisitor::postorder_walk(CallExprAST *ast) {
  generic_postprint();
}
void AstPrinterVisitor::postorder_walk(BinaryExprAST *ast) {
  this->rep += fmt::format("{} Operator: \"{}\"", tabs(), ast->Op->lexeme);

  generic_postprint();
}
void AstPrinterVisitor::postorder_walk(NumberExprAST *ast) {
  generic_postprint();
}
void AstPrinterVisitor::postorder_walk(BoolExprAST *ast) {
  generic_postprint();
}
void AstPrinterVisitor::postorder_walk(VariableExprAST *ast) {
  generic_postprint();
}
void AstPrinterVisitor::postorder_walk(BlockAST *ast) { generic_postprint(); }
void AstPrinterVisitor::postorder_walk(IfExprAST *ast) { generic_postprint(); }
void AstPrinterVisitor::postorder_walk(TypedVarAST *ast) {
  generic_postprint();
}

} // namespace sammine_lang::AST
