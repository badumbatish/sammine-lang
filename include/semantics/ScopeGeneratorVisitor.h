#pragma once
#include "ast/Ast.h"
#include "ast/AstBase.h"
#include "util/LexicalContext.h"
#include "util/Utilities.h"
#include <memory>
#include <stack>
namespace sammine_lang::AST {

class ScopeGeneratorVisitor : public ScopedASTVisitor {
public:
  // A simple scoping class, doesn't differentiate between different names, like
  // variable name, func name and all that
  LexicalStack<sammine_util::Location> scope_stack;
  ScopeGeneratorVisitor() { scope_stack.push_context(); }

  // INFO: CheckAndReg means: Check if there's redefinition, if not, register
  // INFO: Check for castable means: Check if the name existed, if not, register

  virtual void enter_new_scope() override { this->scope_stack.push_context(); }
  virtual void exit_new_scope() override { this->scope_stack.pop_context(); }

  // pre order

  // INFO: Nothing here
  virtual void preorder_walk(ProgramAST *ast) override;

  // INFO: CheckAndReg variable name
  virtual void preorder_walk(VarDefAST *ast) override;

  // INFO: CheckAndReg extern name
  virtual void preorder_walk(ExternAST *ast) override;
  // INFO: CheckAndReg function name, enter new block
  virtual void preorder_walk(FuncDefAST *ast) override;
  // INFO: CheckAndReg all variable name, which should only clash if you have
  // the same names in prototype
  virtual void preorder_walk(PrototypeAST *ast) override;
  // INFO: Check
  virtual void preorder_walk(CallExprAST *ast) override;
  virtual void preorder_walk(ReturnExprAST *ast) override;
  virtual void preorder_walk(BinaryExprAST *ast) override;
  virtual void preorder_walk(NumberExprAST *ast) override;
  virtual void preorder_walk(StringExprAST *ast) override;
  virtual void preorder_walk(BoolExprAST *ast) override;

  // INFO: Check
  virtual void preorder_walk(VariableExprAST *ast) override;
  virtual void preorder_walk(BlockAST *ast) override;
  virtual void preorder_walk(IfExprAST *ast) override;
  virtual void preorder_walk(TypedVarAST *ast) override;

  // post order
  virtual void postorder_walk(ProgramAST *ast) override;
  virtual void postorder_walk(VarDefAST *ast) override;
  virtual void postorder_walk(ExternAST *ast) override;
  // INFO: Pop the scope
  virtual void postorder_walk(FuncDefAST *ast) override;
  virtual void postorder_walk(PrototypeAST *ast) override;
  virtual void postorder_walk(CallExprAST *ast) override;
  virtual void postorder_walk(ReturnExprAST *ast) override;
  virtual void postorder_walk(BinaryExprAST *ast) override;
  virtual void postorder_walk(NumberExprAST *ast) override;
  virtual void postorder_walk(StringExprAST *ast) override;
  virtual void postorder_walk(BoolExprAST *ast) override;
  virtual void postorder_walk(VariableExprAST *ast) override;
  virtual void postorder_walk(BlockAST *ast) override;
  virtual void postorder_walk(IfExprAST *ast) override;
  virtual void postorder_walk(TypedVarAST *ast) override;
};

} // namespace sammine_lang::AST
