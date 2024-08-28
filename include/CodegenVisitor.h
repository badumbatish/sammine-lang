//
// Created by Jasmine Tang on 3/27/24.
//

#ifndef SAMMINE_LANG_CODEGENVISITOR_H
#define SAMMINE_LANG_CODEGENVISITOR_H

#include "Ast.h"
#include "AstBase.h"
#include "LLVMRes.h"
#include <memory>
namespace sammine_lang::AST {
class CgVisitor : public ASTVisitor {
private:
  std::shared_ptr<sammine_lang::LLVMRes> resPtr;

public:
  CgVisitor(std::shared_ptr<sammine_lang::LLVMRes> resPtr) : resPtr(resPtr) {}

  // pre order
  // TODO: Implement these
  void preorder_walk(ProgramAST *ast) override {}
  void preorder_walk(VarDefAST *ast) override {}
  void preorder_walk(FuncDefAST *ast) override {}
  void preorder_walk(PrototypeAST *ast) override {}
  void preorder_walk(CallExprAST *ast) override {}
  void preorder_walk(BinaryExprAST *ast) override {}
  void preorder_walk(NumberExprAST *ast) override {}
  void preorder_walk(VariableExprAST *ast) override {}
  void preorder_walk(BlockAST *ast) override {}
  void preorder_walk(TypedVarAST *ast) override {}

  // post order
  // TODO: Implement these?
  void postorder_walk(ProgramAST *ast) override {}
  void postorder_walk(VarDefAST *ast) override {}
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

#endif // SAMMINE_LANG_CODEGENVISITOR_H
