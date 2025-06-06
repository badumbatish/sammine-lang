#include "semantics/ScopeGeneratorVisitor.h"
#include "ast/Ast.h"
#include "fmt/format.h"
#include "util/LexicalContext.h"
#include "util/Utilities.h"
#include <ctime>
namespace sammine_lang::AST {
// pre order
void ScopeGeneratorVisitor::preorder_walk(ProgramAST *ast) {

  std::string fn_name = "";
  sammine_util::Location loc;
  for (auto &def : ast->DefinitionVec) {
    if (auto fn_def = static_cast<FuncDefAST *>(def.get())) {
      fn_name = fn_def->Prototype->functionName;
      loc = fn_def->Prototype->get_location();
    } else if (auto extern_def = static_cast<ExternAST *>(def.get())) {
      fn_name = extern_def->Prototype->functionName;
      loc = extern_def->Prototype->get_location();
    }

    if (fn_name != "") {
      if (this->scope_stack.recursiveQueryName(fn_name) == nameNotFound)
        this->scope_stack.registerNameT(fn_name, loc);
      else if (this->scope_stack.recursiveQueryName(fn_name) == nameFound) {
        add_error(loc, fmt::format(
                           "[SCOPE1]: The name `{}` has been introduced before",
                           fn_name));
        add_error(this->scope_stack.get_from_name(fn_name),
                  fmt::format("[SCOPE1]: Most recently defined `{}` is here",
                              fn_name));
      }
    }
  }
}
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
void ScopeGeneratorVisitor::preorder_walk(RecordDefAST *ast) {}
void ScopeGeneratorVisitor::preorder_walk(PrototypeAST *ast) {
  // get previous scope and register the function name

  auto *parent_scope = this->scope_stack.parent_scope();

  auto var_name = ast->functionName;
  if (parent_scope->recursiveQueryName(var_name) == nameNotFound)
    parent_scope->registerNameT(var_name, ast->get_location());

  for (auto &param : ast->parameterVectors)
    this->scope_stack.registerNameT(param->name, param->get_location());
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
void ScopeGeneratorVisitor::postorder_walk(RecordDefAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk(PrototypeAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk(CallExprAST *ast) {

  auto var_name = ast->functionName;
  if (this->scope_stack.parent_scope()->recursiveQueryName(var_name) ==
      nameNotFound) {
    add_error(
        ast->get_location(),
        fmt::format(
            "The called name {} for the call expression is not found before",
            var_name));
  }
}

void ScopeGeneratorVisitor::postorder_walk(ReturnExprAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk(BinaryExprAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk(NumberExprAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk(BoolExprAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk(VariableExprAST *ast) {

  auto var_name = ast->variableName;
  if (this->scope_stack.recursiveQueryName(var_name) == nameNotFound) {
    add_error(ast->get_location(),
              fmt::format("The variable named {} for the variable expression "
                          "is not found before",
                          var_name));
  }
}
void ScopeGeneratorVisitor::postorder_walk(BlockAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk(IfExprAST *ast) {}
void ScopeGeneratorVisitor::postorder_walk(TypedVarAST *ast) {}
} // namespace sammine_lang::AST
