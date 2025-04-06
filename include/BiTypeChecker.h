#pragma once

#include "AstBase.h"
#include "LexicalContext.h"
#include "Types.h"
#include <memory>
#include <stack>
#include <variant>
namespace sammine_lang {

namespace AST {

class TypingContext : public LexicalContext<Type> {};
class BiTypeCheckerVisitor : public ASTVisitor {

public:
  std::stack<TypingContext> scope_stack;
  BiTypeCheckerVisitor() { scope_stack.push(TypingContext()); }
  TypingContext get_scope_map();

  // INFO: CheckAndReg means: Check if there's redefinition, if not, register
  // INFO: Check for castable means: Check if the name existed, if not, register

  // pre order

  // INFO: Nothing here
  void preorder_walk(ProgramAST *ast) override;

  // INFO: CheckAndReg variable name
  void preorder_walk(VarDefAST *ast) override;

  // INFO: CheckAndReg extern name
  void preorder_walk(ExternAST *ast) override;
  // INFO: CheckAndReg function name, enter new block
  void preorder_walk(FuncDefAST *ast) override;
  // INFO: CheckAndReg all variable name, which should only clash if you have
  // the same names in prototype
  void preorder_walk(PrototypeAST *ast) override;
  // INFO: Check
  void preorder_walk(CallExprAST *ast) override;
  void preorder_walk(BinaryExprAST *ast) override;
  void preorder_walk(NumberExprAST *ast) override;
  void preorder_walk(BoolExprAST *ast) override;

  // INFO: Check
  void preorder_walk(VariableExprAST *ast) override;
  void preorder_walk(BlockAST *ast) override;
  void preorder_walk(IfExprAST *ast) override;
  void preorder_walk(TypedVarAST *ast) override;

  // post order
  void postorder_walk(ProgramAST *ast) override;
  void postorder_walk(VarDefAST *ast) override;
  void postorder_walk(ExternAST *ast) override;
  // INFO: Pop the scope
  void postorder_walk(FuncDefAST *ast) override;
  void postorder_walk(PrototypeAST *ast) override;
  void postorder_walk(CallExprAST *ast) override;
  void postorder_walk(BinaryExprAST *ast) override;
  void postorder_walk(NumberExprAST *ast) override;
  void postorder_walk(BoolExprAST *ast) override;
  void postorder_walk(VariableExprAST *ast) override;
  void postorder_walk(BlockAST *ast) override;
  void postorder_walk(IfExprAST *ast) override;
  void postorder_walk(TypedVarAST *ast) override;
};
} // namespace AST
} // namespace sammine_lang
