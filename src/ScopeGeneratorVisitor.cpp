#include "ScopeGeneratorVisitor.h"
#include "Ast.h"
#include "Utilities.h"
#include "fmt/format.h"
#include <iostream>
namespace sammine_lang::AST {
// pre order
void ScopeGeneratorVisitor::preorder_walk(ProgramAST *ast) {}
void ScopeGeneratorVisitor::preorder_walk(VarDefAST *ast) {

  std::cerr << "Scope checking VarDefAST\n";
}
void ScopeGeneratorVisitor::preorder_walk(ExternAST *ast) {}
void ScopeGeneratorVisitor::preorder_walk(FuncDefAST *ast) {
  std::cerr << "Scope checking FuncDef\n";
  this->scope_stack.push(LexicalScope(this->scope_stack.top()));
}
void ScopeGeneratorVisitor::preorder_walk(PrototypeAST *ast) {
  auto &scope = this->scope_stack.top();

  auto var_name = ast->functionName;
  if (scope.queryName(var_name) == LexicalScope::nameNotFound) {
    scope.registerName(var_name);
  } else if (scope.queryName(var_name) == LexicalScope::nameFound) {
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
  std::cerr << "Scope checking TypedVarAST\n";
  auto &scope = this->scope_stack.top();

  auto var_name = ast->name;
  std::cerr << "Var name is " << var_name << "\n";
  if (scope.queryName(var_name) == LexicalScope::nameNotFound) {
    std::cerr << "Registed the name " << var_name << "\n";
    scope.registerName(var_name);
  } else if (scope.queryName(var_name) == LexicalScope::nameFound) {
    sammine_util::abort(fmt::format(
        "The name {} for definition has been used before", var_name));
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
