#include "ScopeGeneratorVisitor.h"
#include "Ast.h"
#include "Utilities.h"
#include "fmt/format.h"
using sammine_lang::AST::LexicalScope::nameFound;
using sammine_lang::AST::LexicalScope::nameNotFound;
namespace sammine_lang::AST {
// pre order
void ScopeGeneratorVisitor::preorder_walk(ProgramAST *ast) {}
void ScopeGeneratorVisitor::preorder_walk(VarDefAST *ast) {}
void ScopeGeneratorVisitor::preorder_walk(ExternAST *ast) {
  auto scope = this->scope_stack.top();

  auto var_name = ast->Prototype->functionName;
  if (scope.queryName(var_name) == nameNotFound) {
    scope.registerName(var_name);
  } else if (scope.queryName(var_name) == nameFound) {
    sammine_util::abort(fmt::format(
        "The name {} for externing has been used before", var_name));
  }
}
void ScopeGeneratorVisitor::preorder_walk(FuncDefAST *ast) {
  this->scope_stack.push(LexicalScope(this->scope_stack.top()));
}
void ScopeGeneratorVisitor::preorder_walk(PrototypeAST *ast) {
  auto scope = this->scope_stack.top();

  auto var_name = ast->functionName;
  if (scope.queryName(var_name) == nameNotFound) {
    scope.registerName(var_name);
  } else if (scope.queryName(var_name) == nameFound) {
    sammine_util::abort(fmt::format(
        "The name {} for function prototype has been used before", var_name));
  }
}
void ScopeGeneratorVisitor::preorder_walk(CallExprAST *ast) {}
void ScopeGeneratorVisitor::preorder_walk(BinaryExprAST *ast) {}
void ScopeGeneratorVisitor::preorder_walk(NumberExprAST *ast) {}
void ScopeGeneratorVisitor::preorder_walk(BoolExprAST *ast) {}
void ScopeGeneratorVisitor::preorder_walk(VariableExprAST *ast) {}
void ScopeGeneratorVisitor::preorder_walk(BlockAST *ast) {}
void ScopeGeneratorVisitor::preorder_walk(IfExprAST *ast) {}
void ScopeGeneratorVisitor::preorder_walk(TypedVarAST *ast) {
  auto scope = this->scope_stack.top();

  auto var_name = ast->name;
  if (scope.queryName(var_name) == nameNotFound) {
    scope.registerName(var_name);
  } else if (scope.queryName(var_name) == nameFound) {
    sammine_util::abort(fmt::format(
        "The name {} for variable definition has been used before", var_name));
  }
}

// post order
void ScopeGeneratorVisitor::postorder_walk(ProgramAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk(VarDefAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk(ExternAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk(FuncDefAST *ast) {
  assert(!this->scope_stack.empty());
  this->scope_stack.pop();
}
void ScopeGeneratorVisitor::postorder_walk(PrototypeAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk(CallExprAST *ast) {
  auto scope = this->scope_stack.top();

  auto var_name = ast->functionName;
  if (scope.recursiveQueryName(var_name) == nameNotFound) {
    sammine_util::abort(fmt::format(
        "The called name {} for the call expression is not found before",
        var_name));
  }
}
void ScopeGeneratorVisitor::postorder_walk(BinaryExprAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk(NumberExprAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk(BoolExprAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk(VariableExprAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk(BlockAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk(IfExprAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk(TypedVarAST *ast) {}
} // namespace sammine_lang::AST
