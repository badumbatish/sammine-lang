#pragma once
#include "Ast.h"
#include "AstBase.h"
#include <memory>

#include <map>
#include <set>

namespace sammine_lang::AST {
class LexicalScope {
  std::shared_ptr<LexicalScope> parent_scope;
  std::map<std::string, AST::Visitable &> scope_map;

public:
  explicit LexicalScope() {}
  LexicalScope(std::shared_ptr<LexicalScope> parent_scope)
      : parent_scope(parent_scope) {}
};
class ScopeGeneratorVisitor : public ASTVisitor {
public:
  LexicalScope global_scope;
  std::set<std::string> symbols;
  ScopeGeneratorVisitor() {}
  LexicalScope get_scope_map();

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
};

} // namespace sammine_lang::AST
