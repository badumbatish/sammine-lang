#include "Ast.h"
#include "fmt/base.h"
#include "fmt/format.h"
#include <cstdio>
namespace sammine_lang::AST {

class AstPrinterVisitor : public ScopedASTVisitor {
  std::string rep = "";
  std::string current_tabs = "";
  std::string &tabs();
  void generic_preprintln(const std::string &tree_name);
  void generic_preprint(const std::string &tree_name);
  void generic_postprint();

public:
  virtual void enter_new_scope() override { current_tabs += "  "; }
  virtual void exit_new_scope() override { current_tabs += "  "; }

  virtual void visit(PrototypeAST *ast) override;
  // pre order
  virtual void preorder_walk(ProgramAST *ast) override;
  virtual void preorder_walk(VarDefAST *ast) override;
  virtual void preorder_walk(ExternAST *ast) override;
  virtual void preorder_walk(FuncDefAST *ast) override;
  virtual void preorder_walk(PrototypeAST *ast) override;
  virtual void preorder_walk(CallExprAST *ast) override;
  virtual void preorder_walk(ReturnExprAST *ast) override;
  virtual void preorder_walk(BinaryExprAST *ast) override;
  virtual void preorder_walk(NumberExprAST *ast) override;
  virtual void preorder_walk(BoolExprAST *ast) override;
  virtual void preorder_walk(VariableExprAST *ast) override;
  virtual void preorder_walk(BlockAST *ast) override;
  virtual void preorder_walk(IfExprAST *ast) override;
  virtual void preorder_walk(TypedVarAST *ast) override;

  // post order
  virtual void postorder_walk(ProgramAST *ast) override;
  virtual void postorder_walk(VarDefAST *ast) override;
  virtual void postorder_walk(ExternAST *ast) override;
  virtual void postorder_walk(FuncDefAST *ast) override;
  virtual void postorder_walk(PrototypeAST *ast) override;
  virtual void postorder_walk(CallExprAST *ast) override;
  virtual void postorder_walk(ReturnExprAST *ast) override;
  virtual void postorder_walk(BinaryExprAST *ast) override;
  virtual void postorder_walk(NumberExprAST *ast) override;
  virtual void postorder_walk(BoolExprAST *ast) override;
  virtual void postorder_walk(VariableExprAST *ast) override;
  virtual void postorder_walk(BlockAST *ast) override;
  virtual void postorder_walk(IfExprAST *ast) override;
  virtual void postorder_walk(TypedVarAST *ast) override;

  friend Printable;
};

void ASTPrinter::print(AstBase *ast) {
  auto vs = AstPrinterVisitor();
  ast->accept_vis(&vs);
}
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
void AstPrinterVisitor::preorder_walk(ReturnExprAST *ast) {
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
void AstPrinterVisitor::postorder_walk(ReturnExprAST *ast) {
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
