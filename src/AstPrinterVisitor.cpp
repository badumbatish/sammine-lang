#include "AstPrinterVisitor.h"
#include "fmt/base.h"
#include "fmt/format.h"
#include <cstdio>
namespace sammine_lang::AST {
std::string AstPrinterVisitor::tabs() {
  std::string result;
  for (size_t i = 1; i <= this->tab_counter; i++)
    result += "  ";

  return result;
}

void AstPrinterVisitor::generic_preprint(const std::string &tree_name, Type t) {
  fmt::print("{} {} ", tabs(), tree_name);
  std::printf("{\n");
  tab_counter++;
  fmt::print("{} Type: {} \n", tabs(), t.to_string());
}

void AstPrinterVisitor::generic_postprint() {

  tab_counter--;
  fmt::print("{}", tabs());
  std::printf("}\n");
}
void AstPrinterVisitor::preorder_walk(ProgramAST *ast) {
  generic_preprint(ast->getTreeName(), ast->type);
}
void AstPrinterVisitor::preorder_walk(VarDefAST *ast) {
  generic_preprint(ast->getTreeName(), ast->type);
}
void AstPrinterVisitor::preorder_walk(ExternAST *ast) {
  generic_preprint(ast->getTreeName(), ast->type);
}
void AstPrinterVisitor::preorder_walk(FuncDefAST *ast) {
  generic_preprint(ast->getTreeName(), ast->type);
}
void AstPrinterVisitor::preorder_walk(PrototypeAST *ast) {
  generic_preprint(ast->getTreeName(), ast->type);
}
void AstPrinterVisitor::preorder_walk(CallExprAST *ast) {
  generic_preprint(ast->getTreeName(), ast->type);
}
void AstPrinterVisitor::preorder_walk(BinaryExprAST *ast) {
  generic_preprint(ast->getTreeName(), ast->type);
}
void AstPrinterVisitor::preorder_walk(NumberExprAST *ast) {
  generic_preprint(ast->getTreeName(), ast->type);
}
void AstPrinterVisitor::preorder_walk(BoolExprAST *ast) {
  generic_preprint(ast->getTreeName(), ast->type);
}
void AstPrinterVisitor::preorder_walk(VariableExprAST *ast) {
  generic_preprint(ast->getTreeName(), ast->type);
}
void AstPrinterVisitor::preorder_walk(BlockAST *ast) {
  generic_preprint(ast->getTreeName(), ast->type);
}
void AstPrinterVisitor::preorder_walk(IfExprAST *ast) {
  generic_preprint(ast->getTreeName(), ast->type);
}
void AstPrinterVisitor::preorder_walk(TypedVarAST *ast) {
  generic_preprint(ast->getTreeName(), ast->type);
}

// post order
void AstPrinterVisitor::postorder_walk(ProgramAST *ast) { generic_postprint(); }
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
