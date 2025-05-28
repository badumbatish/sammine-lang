#pragma once

#include "Ast.h"
#include "AstBase.h"
#include "Types.h"

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
  virtual void postorder_walk(BinaryExprAST *ast) override;
  virtual void postorder_walk(NumberExprAST *ast) override;
  virtual void postorder_walk(BoolExprAST *ast) override;
  virtual void postorder_walk(VariableExprAST *ast) override;
  virtual void postorder_walk(BlockAST *ast) override;
  virtual void postorder_walk(IfExprAST *ast) override;
  virtual void postorder_walk(TypedVarAST *ast) override;

  friend Printable;
};

} // namespace sammine_lang::AST
