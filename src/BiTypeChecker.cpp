#include "BiTypeChecker.h"
#include "Ast.h"
#include "Utilities.h"
#include "fmt/format.h"
namespace sammine_lang::AST {
// pre order
void BiTypeCheckerVisitor::preorder_walk(ProgramAST *ast) {}
void BiTypeCheckerVisitor::preorder_walk(VarDefAST *ast) {
  ast->accept_synthesis(this);
}

void BiTypeCheckerVisitor::preorder_walk(ExternAST *ast) {}
void BiTypeCheckerVisitor::preorder_walk(FuncDefAST *ast) {}
void BiTypeCheckerVisitor::preorder_walk(PrototypeAST *ast) {}
void BiTypeCheckerVisitor::preorder_walk(CallExprAST *ast) {}
void BiTypeCheckerVisitor::preorder_walk(ReturnExprAST *ast) {}
void BiTypeCheckerVisitor::preorder_walk(BinaryExprAST *ast) {}
void BiTypeCheckerVisitor::preorder_walk(NumberExprAST *ast) {

  ast->accept_synthesis(this);
}
void BiTypeCheckerVisitor::preorder_walk(BoolExprAST *ast) {}
void BiTypeCheckerVisitor::preorder_walk(VariableExprAST *ast) {
  ast->accept_synthesis(this);
}
void BiTypeCheckerVisitor::preorder_walk(BlockAST *ast) {}
void BiTypeCheckerVisitor::preorder_walk(IfExprAST *ast) {}
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
void BiTypeCheckerVisitor::postorder_walk(FuncDefAST *ast) {}
void BiTypeCheckerVisitor::postorder_walk(PrototypeAST *ast) {
  auto v = std::vector<Type>();
  for (size_t i = 0; i < ast->parameterVectors.size(); i++)
    v.push_back(ast->parameterVectors[i]->type);

  if (ast->returnType.empty())
    v.push_back(Type::Unit());
  else
    v.push_back(
        get_type_from_type_lexeme(ast->returnType, ast->get_location()));

  ast->type = Type::Function(std::move(v));
}
void BiTypeCheckerVisitor::postorder_walk(CallExprAST *ast) {}
void BiTypeCheckerVisitor::postorder_walk(ReturnExprAST *ast) {}
void BiTypeCheckerVisitor::postorder_walk(BinaryExprAST *ast) {}
void BiTypeCheckerVisitor::postorder_walk(NumberExprAST *ast) {}
void BiTypeCheckerVisitor::postorder_walk(BoolExprAST *ast) {}
void BiTypeCheckerVisitor::postorder_walk(VariableExprAST *ast) {}
void BiTypeCheckerVisitor::postorder_walk(BlockAST *ast) {}
void BiTypeCheckerVisitor::postorder_walk(IfExprAST *ast) {}
void BiTypeCheckerVisitor::postorder_walk(TypedVarAST *ast) {}

Type BiTypeCheckerVisitor::synthesize(ProgramAST *ast) {
  return Type::NonExistent();
}
Type BiTypeCheckerVisitor::synthesize(VarDefAST *ast) {
  if (ast->synthesized())
    return ast->type;
  return ast->type = ast->TypedVar->accept_synthesis(this);
}

Type BiTypeCheckerVisitor::synthesize(ExternAST *ast) {
  return Type::NonExistent();
}
Type BiTypeCheckerVisitor::synthesize(FuncDefAST *ast) {
  return Type::NonExistent();
}
Type BiTypeCheckerVisitor::synthesize(PrototypeAST *ast) {
  return Type::NonExistent();
}
Type BiTypeCheckerVisitor::synthesize(CallExprAST *ast) {
  return Type::NonExistent();
}

Type BiTypeCheckerVisitor::synthesize(ReturnExprAST *ast) {
  // TODO: make sure it returns the right type and all that
  return Type::NonExistent();
}
Type BiTypeCheckerVisitor::synthesize(BinaryExprAST *ast) {
  return Type::NonExistent();
}
Type BiTypeCheckerVisitor::synthesize(NumberExprAST *ast) {
  if (ast->synthesized())
    return ast->type;

  sammine_util::abort_on(ast->number.empty(),
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
