#include "AST/ScopeGeneratorVisitor.h"
#include "AST/Ast.h"

namespace sammine_lang::AST {
// pre order
void ScopeGeneratorVisitor::preorder_walk([[maybe_unused]] ProgramAST *ast) {}
void ScopeGeneratorVisitor::preorder_walk([[maybe_unused]] VarDefAST *ast) {}
void ScopeGeneratorVisitor::preorder_walk([[maybe_unused]] ExternAST *ast) {}
void ScopeGeneratorVisitor::preorder_walk([[maybe_unused]] FuncDefAST *ast) {}
void ScopeGeneratorVisitor::preorder_walk([[maybe_unused]] PrototypeAST *ast) {}
void ScopeGeneratorVisitor::preorder_walk([[maybe_unused]] CallExprAST *ast) {}
void ScopeGeneratorVisitor::preorder_walk([[maybe_unused]] BinaryExprAST *ast) {
}
void ScopeGeneratorVisitor::preorder_walk([[maybe_unused]] NumberExprAST *ast) {
}
void ScopeGeneratorVisitor::preorder_walk(
    [[maybe_unused]] VariableExprAST *ast) {}
void ScopeGeneratorVisitor::preorder_walk([[maybe_unused]] BlockAST *ast) {}
void ScopeGeneratorVisitor::preorder_walk([[maybe_unused]] TypedVarAST *ast) {}

// post order
void ScopeGeneratorVisitor::postorder_walk([[maybe_unused]] ProgramAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk([[maybe_unused]] VarDefAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk([[maybe_unused]] ExternAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk([[maybe_unused]] FuncDefAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk([[maybe_unused]] PrototypeAST *ast) {
}
void ScopeGeneratorVisitor::postorder_walk([[maybe_unused]] CallExprAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk(
    [[maybe_unused]] BinaryExprAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk(
    [[maybe_unused]] NumberExprAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk(
    [[maybe_unused]] VariableExprAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk([[maybe_unused]] BlockAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk([[maybe_unused]] TypedVarAST *ast) {}
} // namespace sammine_lang::AST
