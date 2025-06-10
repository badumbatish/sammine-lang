#include "ast/Ast.h"
#include "ast/AstBase.h"

namespace sammine_lang::AST {

ASTVisitor::~ASTVisitor() {}
void ASTVisitor::visit(ProgramAST *ast) {
  ast->walk_with_preorder(this);
  for (auto &def : ast->DefinitionVec) {
    def->accept_vis(this);
  }
  ast->walk_with_postorder(this);
}
void ASTVisitor::visit(VarDefAST *ast) {
  ast->walk_with_preorder(this);
  ast->TypedVar->accept_vis(this);
  ast->Expression->accept_vis(this);
  ast->walk_with_postorder(this);
}

void ASTVisitor::visit(ExternAST *ast) {
  ast->walk_with_preorder(this);
  ast->Prototype->accept_vis(this);
  ast->walk_with_postorder(this);
}
void ASTVisitor::visit(RecordDefAST *ast) {
  ast->walk_with_preorder(this);
  ast->walk_with_postorder(this);
}

void ASTVisitor::visit(FuncDefAST *ast) {
  ast->walk_with_preorder(this);
  ast->Prototype->accept_vis(this);
  ast->Block->accept_vis(this);
  ast->walk_with_postorder(this);
}

void ASTVisitor::visit(PrototypeAST *ast) {
  ast->walk_with_preorder(this);
  for (auto &var : ast->parameterVectors) {
    var->accept_vis(this);
  }
  ast->walk_with_postorder(this);
}

void ASTVisitor::visit(CallExprAST *ast) {
  ast->walk_with_preorder(this);
  for (auto &arg : ast->arguments) {
    arg->accept_vis(this);
  }
  ast->walk_with_postorder(this);
}
void ASTVisitor::visit(ReturnExprAST *ast) {
  ast->walk_with_preorder(this);
  if (ast->return_expr)
    ast->return_expr->accept_vis(this);
  ast->walk_with_postorder(this);
}

void ASTVisitor::visit(BinaryExprAST *ast) {
  ast->walk_with_preorder(this);
  ast->LHS->accept_vis(this);
  ast->RHS->accept_vis(this);
  ast->walk_with_postorder(this);
}

void ASTVisitor::visit(NumberExprAST *ast) {
  ast->walk_with_preorder(this);
  ast->walk_with_postorder(this);
}
void ASTVisitor::visit(StringExprAST *ast) {
  ast->walk_with_preorder(this);
  ast->walk_with_postorder(this);
}

void ASTVisitor::visit(VariableExprAST *ast) {
  ast->walk_with_preorder(this);
  ast->walk_with_postorder(this);
}

void ASTVisitor::visit(UnitExprAST *ast) {
  ast->walk_with_preorder(this);
  ast->walk_with_postorder(this);
}
void ASTVisitor::visit(IfExprAST *ast) {

  ast->walk_with_preorder(this);
  ast->bool_expr->accept_vis(this);
  ast->thenBlockAST->accept_vis(this);
  ast->elseBlockAST->accept_vis(this);
  ast->walk_with_postorder(this);
}

void ASTVisitor::visit(BoolExprAST *ast) {
  ast->walk_with_preorder(this);
  ast->walk_with_postorder(this);
}

void ASTVisitor::visit(BlockAST *ast) {
  ast->walk_with_preorder(this);
  for (auto &stmt : ast->Statements) {
    stmt->accept_vis(this);
  }
  ast->walk_with_postorder(this);
}

void ASTVisitor::visit(TypedVarAST *ast) {
  ast->walk_with_preorder(this);
  ast->walk_with_postorder(this);
}

// -------------------------------------------------------
ScopedASTVisitor::~ScopedASTVisitor() {}

void ScopedASTVisitor::visit(FuncDefAST *ast) {
  this->enter_new_scope();
  ast->walk_with_preorder(this);
  ast->Prototype->accept_vis(this);
  ast->Block->accept_vis(this);
  ast->walk_with_postorder(this);
  this->exit_new_scope();
}

TypeCheckerVisitor::~TypeCheckerVisitor() {}
} // namespace sammine_lang::AST
