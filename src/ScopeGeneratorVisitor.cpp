#include "ScopeGeneratorVisitor.h"
#include "Ast.h"
#include "Utilities.h"
#include "fmt/format.h"
#include <iostream>
namespace sammine_lang::AST {
// pre order
void ScopeGeneratorVisitor::preorder_walk(ProgramAST *ast) {}
void ScopeGeneratorVisitor::preorder_walk(VarDefAST *ast) {

  auto &scope = this->scope_stack.top();

  auto var_name = ast->TypedVar->name;
  if (scope.queryName(var_name) == LexicalScope::nameNotFound) {
    scope.registerNameLocation(var_name, ast->TypedVar->get_location());
  } else if (scope.queryName(var_name) == LexicalScope::nameFound) {
    add_error(ast->get_location(),
              fmt::format("[SCOPE1]: The name `{}` has been introduced before",
                          var_name));
    add_error(
        scope.get_location(var_name),
        fmt::format("[SCOPE1]: Most recently defined `{}` is here", var_name));
  }
}
void ScopeGeneratorVisitor::preorder_walk(ExternAST *ast) {}
void ScopeGeneratorVisitor::preorder_walk(FuncDefAST *ast) {
  this->scope_stack.push(LexicalScope(this->scope_stack.top()));
}
void ScopeGeneratorVisitor::preorder_walk(PrototypeAST *ast) {
  auto &scope = this->scope_stack.top();

  auto var_name = ast->functionName;
  if (scope.queryName(var_name) == LexicalScope::nameNotFound) {
    scope.registerNameLocation(var_name, ast->get_location());
  } else if (scope.queryName(var_name) == LexicalScope::nameFound) {
    add_error(ast->get_location(),
              fmt::format("[SCOPE1]: The name `{}` has been introduced before",
                          var_name));
    add_error(
        scope.get_location(var_name),
        fmt::format("[SCOPE1]: Most recently defined `{}` is here", var_name));
  }
  for (auto &param : ast->parameterVectors) {
    scope.registerNameLocation(param->name, param->get_location());
  }
}
void ScopeGeneratorVisitor::preorder_walk(CallExprAST *ast) {}
void ScopeGeneratorVisitor::preorder_walk(BinaryExprAST *ast) {}
void ScopeGeneratorVisitor::preorder_walk(NumberExprAST *ast) {}
void ScopeGeneratorVisitor::preorder_walk(BoolExprAST *ast) {}
void ScopeGeneratorVisitor::preorder_walk(VariableExprAST *ast) {}
void ScopeGeneratorVisitor::preorder_walk(BlockAST *ast) {}
void ScopeGeneratorVisitor::preorder_walk(IfExprAST *ast) {}
void ScopeGeneratorVisitor::preorder_walk(TypedVarAST *ast) {}

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
  auto &scope = this->scope_stack.top();

  auto var_name = ast->functionName;
  if (scope.recursiveQueryName(var_name) == LexicalScope::nameNotFound) {
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
