//
// Created by Jasmine Tang on 3/27/24.
//

#include "CodegenVisitor.h"

namespace sammine_lang::AST {
void CgVisitor::visit(ProgramAST *ast) {
  for(auto& def : ast->DefinitionVec) {
    def->accept_vis(this);
  }
}
void CgVisitor::visit(VarDefAST *ast) {

}
void CgVisitor::visit(FuncDefAST *ast) {}
void CgVisitor::visit(CallExprAST *ast) {}
void CgVisitor::visit(BinaryExprAST *ast) {
  ast->LHS->accept_vis(this);
  ast->RHS->accept_vis(this);

  auto L = ast->LHS->val;
  auto R = ast->RHS->val;

  if (ast->Op->type == TokenType::TokADD) {
    ast->val = resPtr->Builder->CreateFAdd(L, R, "addtmp");
  }
  if (ast->Op->type == TokenType::TokSUB) {
    ast->val = resPtr->Builder->CreateFSub(L, R, "subtmp");
  }
  if (ast->Op->type == TokenType::TokMUL) {
    ast->val =  resPtr->Builder->CreateFMul(L, R, "multmp");
  }
  if (ast->Op->type == TokenType::TokSUB) {
    ast->val =  resPtr->Builder->CreateFDiv(L, R, "multmp");
  }
  if (ast->Op->type == TokenType::TokLESS) {
    auto cmp_int = resPtr->Builder->CreateFCmpULT(L, R, "cmptmp");
    ast->val = resPtr->Builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*(resPtr->Context)),
                                          "booltmp");

  }
}
void CgVisitor::visit(NumberExprAST *ast) {
  ast->val = llvm::ConstantFP::get(*resPtr->Context, llvm::APFloat(std::stod(ast->number)));
}
void CgVisitor::visit(VariableExprAST *ast) {}
void CgVisitor::visit(BlockAST *ast) {}
}
