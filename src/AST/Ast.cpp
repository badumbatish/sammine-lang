#include "AST/Ast.h"
#include "AST/AstBase.h"

namespace sammine_lang::AST {
void ASTVisitor::visit(ProgramAST *ast) {
  ast->walk_with_preorder(this);
  for (auto &def : ast->DefinitionVec) {
    def->accept_vis(this);
  }
  ast->walk_with_postorder(this);
}
void ASTVisitor::preorder_walk([[maybe_unused]] ProgramAST *ast) {}
void ASTVisitor::postorder_walk([[maybe_unused]] ProgramAST *ast) {}

void ASTVisitor::visit(VarDefAST *ast) {
  ast->walk_with_preorder(this);
  ast->TypedVar->accept_vis(this);
  ast->Expression->accept_vis(this);
  ast->walk_with_postorder(this);
}
void ASTVisitor::preorder_walk([[maybe_unused]] VarDefAST *ast) {}
void ASTVisitor::postorder_walk([[maybe_unused]] VarDefAST *ast) {}

void ASTVisitor::visit(ExternAST *ast) {
  ast->walk_with_preorder(this);
  ast->Prototype->accept_vis(this);
  ast->walk_with_postorder(this);
}
void ASTVisitor::preorder_walk([[maybe_unused]] ExternAST *ast) {}
void ASTVisitor::postorder_walk([[maybe_unused]] ExternAST *ast) {}

void ASTVisitor::visit(FuncDefAST *ast) {
  ast->walk_with_preorder(this);
  ast->Prototype->accept_vis(this);
  ast->Block->accept_vis(this);
  ast->walk_with_postorder(this);
}
void ASTVisitor::preorder_walk([[maybe_unused]] FuncDefAST *ast) {}
void ASTVisitor::postorder_walk([[maybe_unused]] FuncDefAST *ast) {}

void ASTVisitor::visit(PrototypeAST *ast) {
  ast->walk_with_preorder(this);
  for (auto &var : *ast->parameterVectors.get()) {
    var->accept_vis(this);
  }
  ast->walk_with_postorder(this);
}

void ASTVisitor::preorder_walk([[maybe_unused]] PrototypeAST *ast) {}
void ASTVisitor::postorder_walk([[maybe_unused]] PrototypeAST *ast) {}

void ASTVisitor::visit(CallExprAST *ast) {
  ast->walk_with_preorder(this);
  for (auto &arg : *ast->arguments.get()) {
    arg->accept_vis(this);
  }
  ast->walk_with_postorder(this);
}
void ASTVisitor::preorder_walk([[maybe_unused]] CallExprAST *ast) {}
void ASTVisitor::postorder_walk([[maybe_unused]] CallExprAST *ast) {}

void ASTVisitor::visit(BinaryExprAST *ast) {
  ast->walk_with_preorder(this);
  ast->LHS->accept_vis(this);
  ast->RHS->accept_vis(this);
  ast->walk_with_postorder(this);
}
void ASTVisitor::preorder_walk([[maybe_unused]] BinaryExprAST *ast) {}
void ASTVisitor::postorder_walk([[maybe_unused]] BinaryExprAST *ast) {}

void ASTVisitor::visit(NumberExprAST *ast) {
  ast->walk_with_preorder(this);
  ast->walk_with_postorder(this);
}
void ASTVisitor::preorder_walk([[maybe_unused]] NumberExprAST *ast) {}
void ASTVisitor::postorder_walk([[maybe_unused]] NumberExprAST *ast) {}

void ASTVisitor::visit(VariableExprAST *ast) {
  ast->walk_with_preorder(this);
  ast->walk_with_postorder(this);
}
void ASTVisitor::preorder_walk([[maybe_unused]] VariableExprAST *ast) {}
void ASTVisitor::postorder_walk([[maybe_unused]] VariableExprAST *ast) {}

void ASTVisitor::visit(BlockAST *ast) {
  ast->walk_with_preorder(this);
  for (auto &stmt : ast->Statements) {
    stmt->accept_vis(this);
  }
  ast->walk_with_postorder(this);
}
void ASTVisitor::preorder_walk([[maybe_unused]] BlockAST *ast) {}
void ASTVisitor::postorder_walk([[maybe_unused]] BlockAST *ast) {}

void ASTVisitor::visit(TypedVarAST *ast) {
  ast->walk_with_preorder(this);
  ast->walk_with_postorder(this);
}
void ASTVisitor::preorder_walk([[maybe_unused]] TypedVarAST *ast) {}
void ASTVisitor::postorder_walk([[maybe_unused]] TypedVarAST *ast) {}
} // namespace sammine_lang::AST
