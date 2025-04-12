
#pragma once
#include "Ast.h"
#include "AstBase.h"

namespace sammine_lang::AST {
class AstNameVisitor : public ASTVisitor {
private:
public:
  std::vector<std::string> PreOrderNames;
  std::vector<std::string> PostOrderNames;
  AstNameVisitor() {}

  // pre order
  virtual void preorder_walk(ProgramAST *ast) override {
    PreOrderNames.push_back(ast->getTreeName());
  }
  virtual void preorder_walk(VarDefAST *ast) override {
    PreOrderNames.push_back(ast->getTreeName());
  }
  virtual void preorder_walk(ExternAST *ast) override {
    PreOrderNames.push_back(ast->getTreeName());
  }
  virtual void preorder_walk(FuncDefAST *ast) override {
    PreOrderNames.push_back(ast->getTreeName());
  }
  virtual void preorder_walk(PrototypeAST *ast) override {
    PreOrderNames.push_back(ast->getTreeName());
  }
  virtual void preorder_walk(CallExprAST *ast) override {
    PreOrderNames.push_back(ast->getTreeName());
  }
  virtual void preorder_walk(BinaryExprAST *ast) override {
    PreOrderNames.push_back(ast->getTreeName());
  }
  virtual void preorder_walk(NumberExprAST *ast) override {
    PreOrderNames.push_back(ast->getTreeName());
  }
  virtual void preorder_walk(BoolExprAST *ast) override {
    PreOrderNames.push_back(ast->getTreeName());
  }
  virtual void preorder_walk(VariableExprAST *ast) override {
    PreOrderNames.push_back(ast->getTreeName());
  }
  virtual void preorder_walk(BlockAST *ast) override {
    PreOrderNames.push_back(ast->getTreeName());
  }
  virtual void preorder_walk(IfExprAST *ast) override {
    PreOrderNames.push_back(ast->getTreeName());
  }
  virtual void preorder_walk(TypedVarAST *ast) override {
    PreOrderNames.push_back(ast->getTreeName());
  }

  // post order
  virtual void postorder_walk(ProgramAST *ast) override {}
  virtual void postorder_walk(VarDefAST *ast) override {}
  virtual void postorder_walk(ExternAST *ast) override {}
  virtual void postorder_walk(FuncDefAST *ast) override {}
  virtual void postorder_walk(PrototypeAST *ast) override {}
  virtual void postorder_walk(CallExprAST *ast) override {}
  virtual void postorder_walk(BinaryExprAST *ast) override {}
  virtual void postorder_walk(NumberExprAST *ast) override {}
  virtual void postorder_walk(BoolExprAST *ast) override {}
  virtual void postorder_walk(VariableExprAST *ast) override {}
  virtual void postorder_walk(BlockAST *ast) override {}
  virtual void postorder_walk(IfExprAST *ast) override {}
  virtual void postorder_walk(TypedVarAST *ast) override {}
};
} // namespace sammine_lang::AST
