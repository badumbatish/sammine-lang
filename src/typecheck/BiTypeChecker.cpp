#include "typecheck/BiTypeChecker.h"
#include "ast/Ast.h"
#include "fmt/format.h"
#include "typecheck/Types.h"
#include "util/LexicalContext.h"
#include "util/Utilities.h"
#include <ranges>
namespace sammine_lang::AST {
// pre order
void BiTypeCheckerVisitor::preorder_walk(ProgramAST *ast) {}
void BiTypeCheckerVisitor::preorder_walk(VarDefAST *ast) {
  ast->accept_synthesis(this);
}

void BiTypeCheckerVisitor::preorder_walk(ExternAST *ast) {}
void BiTypeCheckerVisitor::preorder_walk(FuncDefAST *ast) {}
void BiTypeCheckerVisitor::preorder_walk(RecordDefAST *ast) {}
void BiTypeCheckerVisitor::preorder_walk(PrototypeAST *ast) {
  ast->accept_synthesis(this);
}
void BiTypeCheckerVisitor::preorder_walk(CallExprAST *ast) {
  ast->accept_synthesis(this);
}
void BiTypeCheckerVisitor::preorder_walk(ReturnExprAST *ast) {
  ast->accept_synthesis(this);
}
void BiTypeCheckerVisitor::preorder_walk(BinaryExprAST *ast) {}
void BiTypeCheckerVisitor::preorder_walk(StringExprAST *ast) {
  ast->accept_synthesis(this);
}
void BiTypeCheckerVisitor::preorder_walk(NumberExprAST *ast) {

  ast->accept_synthesis(this);
}
void BiTypeCheckerVisitor::preorder_walk(BoolExprAST *ast) {}
void BiTypeCheckerVisitor::preorder_walk(VariableExprAST *ast) {
  ast->accept_synthesis(this);
}
void BiTypeCheckerVisitor::preorder_walk(BlockAST *ast) {}
void BiTypeCheckerVisitor::preorder_walk(IfExprAST *ast) {}
void BiTypeCheckerVisitor::preorder_walk(UnitExprAST *ast) {}
void BiTypeCheckerVisitor::preorder_walk(TypedVarAST *ast) {
  ast->accept_synthesis(this);
}

// post order
void BiTypeCheckerVisitor::postorder_walk(ProgramAST *ast) {}
void BiTypeCheckerVisitor::postorder_walk(VarDefAST *ast) {
  if (ast->checked())
    return;

  auto to = ast->accept_synthesis(this);
  auto from = ast->Expression->accept_synthesis(this);
  if (!type_map_ordering.compatible_to_from(to, from)) {
    this->add_error(
        ast->get_location(),
        fmt::format("Type mismatch in variable definition: Synthesized {}, "
                    "checked against {}.",
                    to.to_string(), from.to_string()));
    ast->type = Type::Poisoned();
  }
  ast->set_checked();
}
void BiTypeCheckerVisitor::postorder_walk(ExternAST *ast) {}
void BiTypeCheckerVisitor::postorder_walk(RecordDefAST *ast) {}
void BiTypeCheckerVisitor::postorder_walk(FuncDefAST *ast) {
  if (ast->checked())
    return;

  ast->set_checked();
}

void BiTypeCheckerVisitor::postorder_walk(PrototypeAST *ast) {
  id_to_type.parent_scope()->registerNameT(ast->functionName, ast->type);
}
void BiTypeCheckerVisitor::postorder_walk(CallExprAST *ast) {
  if (ast->checked())
    return;

  auto ty = get_type_from_id_parent(ast->functionName);
  auto func = std::get<FunctionType>(ty->type_data);
  auto params = func.get_params_types();
  if (ast->arguments.size() != params.size()) {
    this->add_error(
        ast->get_location(),
        fmt::format("Function '{}' params and arguments have a type mismatch",
                    ast->functionName));
  }

  for (const auto &[arg, par] :
       std::ranges::views::zip(ast->arguments, params)) {
    if (!this->type_map_ordering.compatible_to_from(par, arg->type)) {
      this->add_error(
          ast->get_location(),
          fmt::format("Function '{}' params and arguments have a type mismatch",
                      ast->functionName));
    }
  }

  ast->set_checked();
}
void BiTypeCheckerVisitor::postorder_walk(ReturnExprAST *ast) {}
void BiTypeCheckerVisitor::postorder_walk(BinaryExprAST *ast) {
  ast->accept_synthesis(this);
}
void BiTypeCheckerVisitor::postorder_walk(StringExprAST *ast) {}
void BiTypeCheckerVisitor::postorder_walk(NumberExprAST *ast) {}
void BiTypeCheckerVisitor::postorder_walk(BoolExprAST *ast) {}
void BiTypeCheckerVisitor::postorder_walk(VariableExprAST *ast) {}
void BiTypeCheckerVisitor::postorder_walk(BlockAST *ast) {}
void BiTypeCheckerVisitor::postorder_walk(IfExprAST *ast) {}
void BiTypeCheckerVisitor::postorder_walk(UnitExprAST *ast) {}
void BiTypeCheckerVisitor::postorder_walk(TypedVarAST *ast) {}

Type BiTypeCheckerVisitor::synthesize(ProgramAST *ast) {
  return Type::NonExistent();
}

Type BiTypeCheckerVisitor::synthesize(VarDefAST *ast) {
  if (ast->synthesized())
    return ast->type;

  // if you dont have type lexeme for typed var, then just assign type of expr
  // to typed var, if we dont have expr also, then we add error
  //
  // if you do, then just use type lexeme as type of typed var

  if (!ast->TypedVar->type_lexeme.empty())
    ast->type = ast->TypedVar->accept_synthesis(this);
  else if (ast->Expression)
    ast->type = ast->Expression->accept_synthesis(this);
  else {
    this->add_error(ast->get_location(),
                    "Variable declared without initializer");
    ast->type = Type::Poisoned();
  }

  id_to_type.registerNameT(ast->TypedVar->name, ast->type);
  return ast->type;
}

Type BiTypeCheckerVisitor::synthesize(ExternAST *ast) {
  return Type::NonExistent();
}
Type BiTypeCheckerVisitor::synthesize(RecordDefAST *ast) {
  return Type::NonExistent();
}
Type BiTypeCheckerVisitor::synthesize(FuncDefAST *ast) {
  if (ast->synthesized())
    return ast->type;

  return ast->type = ast->Prototype->accept_synthesis(this);
}

Type BiTypeCheckerVisitor::synthesize(PrototypeAST *ast) {
  auto v = std::vector<Type>();
  for (size_t i = 0; i < ast->parameterVectors.size(); i++)
    v.push_back(ast->parameterVectors[i]->accept_synthesis(this));

  if (ast->returnType.empty())
    v.push_back(Type::Unit());
  else
    v.push_back(
        get_type_from_type_lexeme(ast->returnType, ast->get_location()));
  ast->type = Type::Function(std::move(v));

  return ast->type;
}

Type BiTypeCheckerVisitor::synthesize(CallExprAST *ast) {
  if (ast->synthesized())
    return ast->type;
  auto ty = get_type_from_id_parent(ast->functionName);
  switch (ty->type_kind) {
  case TypeKind::Function:
    return ast->type = std::get<FunctionType>(ty->type_data).get_return_type();
  case TypeKind::String:
    this->add_error(ast->get_location(),
                    "A string cannot be in place of a call expression");
    return Type::Poisoned();
  case TypeKind::I64_t:
  case TypeKind::F64_t:
  case TypeKind::Unit:
  case TypeKind::Bool:
  case TypeKind::NonExistent:
  case TypeKind::Poisoned:
    this->abort(fmt::format("should not happen here with function {}",
                                    ast->functionName));
  }
  return Type::NonExistent();
}

Type BiTypeCheckerVisitor::synthesize(ReturnExprAST *ast) {
  // TODO: make sure it returns the right type and all that
  return ast->type = ast->return_expr->accept_synthesis(this);
}
Type BiTypeCheckerVisitor::synthesize(BinaryExprAST *ast) {
  if (ast->synthesized())
    return ast->type;
  if (!this->type_map_ordering.compatible_to_from(ast->LHS->type,
                                                  ast->RHS->type))
    this->abort();
  if (ast->Op->is_comparison())
    return ast->type = Type::Bool();

  return ast->type = ast->LHS->type;
}

Type BiTypeCheckerVisitor::synthesize(StringExprAST *ast) {
  if (ast->synthesized())
    return ast->type;
  return ast->type = Type::String(ast->string_content);
}

Type BiTypeCheckerVisitor::synthesize(NumberExprAST *ast) {
  if (ast->synthesized())
    return ast->type;

  this->abort_on(ast->number.empty(),
                         "NumberExprAST should have a number lexeme");
  if (ast->number.find('.') == std::string::npos)
    ast->type = Type::I64_t();
  else
    ast->type = Type::F64_t();

  return ast->type;
}
Type BiTypeCheckerVisitor::synthesize(BoolExprAST *ast) {
  return Type::NonExistent();
}
Type BiTypeCheckerVisitor::synthesize(VariableExprAST *ast) {
  return ast->type = id_to_type.get_from_name(ast->variableName);
}
Type BiTypeCheckerVisitor::synthesize(BlockAST *ast) {
  return Type::NonExistent();
}
Type BiTypeCheckerVisitor::synthesize(UnitExprAST *ast) {
  if (ast->synthesized())
    return ast->type;
  return ast->type = Type::Unit();
}
Type BiTypeCheckerVisitor::synthesize(IfExprAST *ast) {
  return Type::NonExistent();
}
Type BiTypeCheckerVisitor::synthesize(TypedVarAST *ast) {
  if (ast->synthesized())
    return ast->type;
  auto ast_type =
      get_type_from_type_lexeme(ast->type_lexeme, ast->get_location());

  id_to_type.registerNameT(ast->name, ast_type);
  return ast->type = ast_type;
}

} // namespace sammine_lang::AST
