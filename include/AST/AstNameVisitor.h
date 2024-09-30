
#ifndef SAMMINE_LANG_ASTNAMEVISITOR_H
#define SAMMINE_LANG_ASTNAMEVISITOR_H
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
  void preorder_walk(ProgramAST *ast) override {
    PreOrderNames.push_back(ast->getTreeName());
  }
  void preorder_walk(VarDefAST *ast) override {
    PreOrderNames.push_back(ast->getTreeName());
  }
  void preorder_walk(ExternAST *ast) override {
    PreOrderNames.push_back(ast->getTreeName());
  }
  void preorder_walk(FuncDefAST *ast) override {
    PreOrderNames.push_back(ast->getTreeName());
  }
  void preorder_walk(PrototypeAST *ast) override {
    PreOrderNames.push_back(ast->getTreeName());
  }
  void preorder_walk(CallExprAST *ast) override {
    PreOrderNames.push_back(ast->getTreeName());
  }
  void preorder_walk(BinaryExprAST *ast) override {
    PreOrderNames.push_back(ast->getTreeName());
  }
  void preorder_walk(NumberExprAST *ast) override {
    PreOrderNames.push_back(ast->getTreeName());
  }
  void preorder_walk(VariableExprAST *ast) override {
    PreOrderNames.push_back(ast->getTreeName());
  }
  void preorder_walk(BlockAST *ast) override {
    PreOrderNames.push_back(ast->getTreeName());
  }
  void preorder_walk(TypedVarAST *ast) override {
    PreOrderNames.push_back(ast->getTreeName());
  }

  // post order
  void postorder_walk(ProgramAST *ast) override {}
  void postorder_walk(VarDefAST *ast) override {}
  void postorder_walk(ExternAST *ast) override {}
  void postorder_walk(FuncDefAST *ast) override {}
  void postorder_walk(PrototypeAST *ast) override {}
  void postorder_walk(CallExprAST *ast) override {}
  void postorder_walk(BinaryExprAST *ast) override {}
  void postorder_walk(NumberExprAST *ast) override {}
  void postorder_walk(VariableExprAST *ast) override {}
  void postorder_walk(BlockAST *ast) override {}
  void postorder_walk(TypedVarAST *ast) override {}
};
} // namespace sammine_lang::AST
#endif // SAMMINE_LANG_ASTNAMEVISITOR_H
