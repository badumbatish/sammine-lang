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
void BiTypeCheckerVisitor::preorder_walk(BinaryExprAST *ast) {}
void BiTypeCheckerVisitor::preorder_walk(NumberExprAST *ast) {

  ast->accept_synthesis(this);
}
void BiTypeCheckerVisitor::preorder_walk(BoolExprAST *ast) {}
void BiTypeCheckerVisitor::preorder_walk(VariableExprAST *ast) {}
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

  if (ast->type != ast->Expression->type) {
    this->add_error(
        ast->get_location(),
        fmt::format("Type mismatch in variable definition: Synthesized {}, "
                    "checked against {}.",
                    ast->accept_synthesis(this).to_string(),
                    ast->Expression->accept_synthesis(this).to_string()));
    ast->type = Type::Poisoned();
  }
  ast->set_checked();
}
void BiTypeCheckerVisitor::postorder_walk(ExternAST *ast) {}
void BiTypeCheckerVisitor::postorder_walk(FuncDefAST *ast) {}
void BiTypeCheckerVisitor::postorder_walk(PrototypeAST *ast) {}
void BiTypeCheckerVisitor::postorder_walk(CallExprAST *ast) {}
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
Type BiTypeCheckerVisitor::synthesize(BinaryExprAST *ast) {
  return Type::NonExistent();
}
Type BiTypeCheckerVisitor::synthesize(NumberExprAST *ast) {
  if (ast->synthesized())
    return ast->type;

  sammine_util::abort_on(ast->number.empty(),
                         "NumberExprAST should have a number lexeme");
  if (ast->number.find('.') != std::string::npos)
    ast->type = Type::F64_t();
  else
    ast->type = Type::I64_t();

  return ast->type;
}
Type BiTypeCheckerVisitor::synthesize(BoolExprAST *ast) {
  return Type::NonExistent();
}
Type BiTypeCheckerVisitor::synthesize(VariableExprAST *ast) {
  return Type::NonExistent();
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

  if (ast->type_lexeme.empty()) {
    ast->type = Type::NonExistent();
    return ast->type;
  }
  auto get_type_opt = this->get_typename_type(ast->type_lexeme);

  if (!get_type_opt.has_value()) {
    this->add_error(ast->get_location(),
                    fmt::format("Type '{}' not found in the current scope.",
                                ast->type_lexeme));
    ast->type = Type::Poisoned();
  } else
    ast->type = get_type_opt.value();
  return ast->type;
}

} // namespace sammine_lang::AST
