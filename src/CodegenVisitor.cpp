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
void CgVisitor::visit(BinaryExprAST *ast) {}
void CgVisitor::visit(NumberExprAST *ast) {
  ast->val = llvm::ConstantFP::get(*resPtr->Context, llvm::APFloat(std::stod(ast->number)));
}
void CgVisitor::visit(VariableExprAST *ast) {}
void CgVisitor::visit(BlockAST *ast) {}
}
