#include "ScopeGeneratorVisitor.h"
#include "Ast.h"
#include "Utilities.h"
#include "fmt/format.h"
namespace sammine_lang::AST {
// pre order
void ScopeGeneratorVisitor::preorder_walk(ProgramAST *ast) {}
void ScopeGeneratorVisitor::preorder_walk(VarDefAST *ast) {

  auto var_name = ast->TypedVar->name;
  if (this->scope_stack.recursiveQueryName(var_name) == nameNotFound) {
    this->scope_stack.registerNameT(var_name, ast->TypedVar->get_location());
  } else if (this->scope_stack.recursiveQueryName(var_name) == nameFound) {
    add_error(ast->get_location(),
              fmt::format("[SCOPE1]: The name `{}` has been introduced before",
                          var_name));
    add_error(
        this->scope_stack.recursive_get_from_name(var_name),
        fmt::format("[SCOPE1]: The firstly defined `{}` is here", var_name));
  }
}
void ScopeGeneratorVisitor::preorder_walk(ExternAST *ast) {}
void ScopeGeneratorVisitor::preorder_walk(FuncDefAST *ast) {}
void ScopeGeneratorVisitor::preorder_walk(PrototypeAST *ast) {
  // get previous scope and register the function name

  auto *parent_scope = this->scope_stack.parent_scope();

  auto var_name = ast->functionName;
  if (parent_scope->recursiveQueryName(var_name) == nameNotFound) {
    parent_scope->registerNameT(var_name, ast->get_location());
  } else if (parent_scope->recursiveQueryName(var_name) == nameFound) {
    add_error(ast->get_location(),
              fmt::format("[SCOPE1]: The name `{}` has been introduced before",
                          var_name));
    add_error(
        parent_scope->recursive_get_from_name(var_name),
        fmt::format("[SCOPE1]: Most recently defined `{}` is here", var_name));
  }
  for (auto &param : ast->parameterVectors) {
    this->scope_stack.registerNameT(param->name, param->get_location());
  }
}
void ScopeGeneratorVisitor::preorder_walk(CallExprAST *ast) {}
void ScopeGeneratorVisitor::preorder_walk(ReturnExprAST *ast) {}
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
void ScopeGeneratorVisitor::postorder_walk(FuncDefAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk(PrototypeAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk(CallExprAST *ast) {

  auto var_name = ast->functionName;
  if (this->scope_stack.recursiveQueryName(var_name) == nameNotFound) {
    sammine_util::abort(fmt::format(
        "The called name {} for the call expression is not found before",
        var_name));
  }
}

void ScopeGeneratorVisitor::postorder_walk(ReturnExprAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk(BinaryExprAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk(NumberExprAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk(BoolExprAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk(VariableExprAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk(BlockAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk(IfExprAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk(TypedVarAST *ast) {}
} // namespace sammine_lang::AST
