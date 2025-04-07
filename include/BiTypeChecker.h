#pragma once

#include "Ast.h"
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

  // We're gonna provide look up in different

public:
  std::stack<TypingContext> id_to_type;
  std::stack<TypingContext> typename_to_type;

  void push_new_scopes() {
    id_to_type.push(TypingContext());
    typename_to_type.push(TypingContext());
  }
  void pop_scopes() {
    id_to_type.pop();
    typename_to_type.pop();
  }
  BiTypeCheckerVisitor() { this->push_new_scopes(); }
  TypingContext &get_id_map() { return id_to_type.top(); }
  TypingContext &get_typename_map() { return typename_to_type.top(); }

  // pre order

  void preorder_walk(ProgramAST *ast) override;
  void preorder_walk(VarDefAST *ast) override;
  void preorder_walk(ExternAST *ast) override;
  void preorder_walk(FuncDefAST *ast) override;
  void preorder_walk(PrototypeAST *ast) override;
  void preorder_walk(CallExprAST *ast) override;
  void preorder_walk(BinaryExprAST *ast) override;
  void preorder_walk(NumberExprAST *ast) override;
  void preorder_walk(BoolExprAST *ast) override;
  void preorder_walk(VariableExprAST *ast) override;
  void preorder_walk(BlockAST *ast) override;
  void preorder_walk(IfExprAST *ast) override;
  void preorder_walk(TypedVarAST *ast) override;

  // post order
  void postorder_walk(ProgramAST *ast) override;
  void postorder_walk(VarDefAST *ast) override;
  void postorder_walk(ExternAST *ast) override;
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

  // INFO: This is for internal usage
  Type infer(ProgramAST *ast);
  Type infer(VarDefAST *ast);
  Type infer(ExternAST *ast);
  Type infer(FuncDefAST *ast);
  Type infer(PrototypeAST *ast);
  Type infer(CallExprAST *ast);
  Type infer(BinaryExprAST *ast);
  Type infer(NumberExprAST *ast);
  Type infer(BoolExprAST *ast);
  Type infer(VariableExprAST *ast);
  Type infer(BlockAST *ast);
  Type infer(IfExprAST *ast);
  Type infer(TypedVarAST *ast);

  bool check(ProgramAST *ast);
  bool check(VarDefAST *ast);
  bool check(ExternAST *ast);
  bool check(FuncDefAST *ast);
  bool check(PrototypeAST *ast);
  bool check(CallExprAST *ast);
  bool check(BinaryExprAST *ast);
  bool check(NumberExprAST *ast);
  bool check(BoolExprAST *ast);
  bool check(VariableExprAST *ast);
  bool check(BlockAST *ast);
  bool check(IfExprAST *ast);
  bool check(TypedVarAST *ast);
};
} // namespace AST
} // namespace sammine_lang
