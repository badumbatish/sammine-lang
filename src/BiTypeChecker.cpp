#include "BiTypeChecker.h"
#include "Ast.h"
#include "Utilities.h"
#include "fmt/format.h"
namespace sammine_lang::AST {
// pre order
void BiTypeCheckerVisitor::preorder_walk(ProgramAST *ast) {}
void BiTypeCheckerVisitor::preorder_walk(VarDefAST *ast) {
  /**/
  /*auto &id_map = this->get_id_map();*/
  /*auto &typename_map = this->get_typename_map();*/
  /**/
  /*// INFO: Case 1 (Only case for now): let x : f64 = ...;*/
  /**/
  /*// let's get the type var*/
  /*auto type_name = ast->TypedVar->type;*/
  /**/
  /*auto &expr = ast->Expression;*/
}
void BiTypeCheckerVisitor::preorder_walk(ExternAST *ast) {}
void BiTypeCheckerVisitor::preorder_walk(FuncDefAST *ast) {}
void BiTypeCheckerVisitor::preorder_walk(PrototypeAST *ast) {}
void BiTypeCheckerVisitor::preorder_walk(CallExprAST *ast) {}
void BiTypeCheckerVisitor::preorder_walk(BinaryExprAST *ast) {}
void BiTypeCheckerVisitor::preorder_walk(NumberExprAST *ast) {}
void BiTypeCheckerVisitor::preorder_walk(BoolExprAST *ast) {}
void BiTypeCheckerVisitor::preorder_walk(VariableExprAST *ast) {}
void BiTypeCheckerVisitor::preorder_walk(BlockAST *ast) {}
void BiTypeCheckerVisitor::preorder_walk(IfExprAST *ast) {}
void BiTypeCheckerVisitor::preorder_walk(TypedVarAST *ast) {}

// post order
void BiTypeCheckerVisitor::postorder_walk(ProgramAST *ast) {}
void BiTypeCheckerVisitor::postorder_walk(VarDefAST *ast) {}
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
  if (ast->type == Type::NonExistent())
    return ast->type = this->synthesize(ast->TypedVar.get());
  return ast->type;
}
Type BiTypeCheckerVisitor::synthesize(ExternAST *ast) {
  if (ast->type == Type::NonExistent())
    return ast->type = this->synthesize(ast->Prototype.get());
  return ast->type;
}
Type BiTypeCheckerVisitor::synthesize(FuncDefAST *ast) {
  if (ast->type == Type::NonExistent())
    return ast->type = this->synthesize(ast->Prototype.get());
  return ast->type;
}
Type BiTypeCheckerVisitor::synthesize(PrototypeAST *ast) {
  if (ast->type != Type::NonExistent())
    return ast->type;
  std::vector<Type> types;
  for (auto &param : ast->parameterVectors) {
    types.push_back(this->synthesize(param.get()));
  }
  types.push_back(this->synthesize_typename(ast->returnType));

  return ast->type = Type::Function(types);
}
Type BiTypeCheckerVisitor::synthesize(CallExprAST *ast) {
  // INFO: Look up the call expr in the id map, and get out the type
  return synthesize_id(ast->functionName);
}
Type BiTypeCheckerVisitor::synthesize(BinaryExprAST *ast) {
  return Type::I64_t();
}
Type BiTypeCheckerVisitor::synthesize(NumberExprAST *ast) {
  if (ast->number.find(".") == std::string::npos)
    return Type::I64_t();
  return Type::F64_t();
}
Type BiTypeCheckerVisitor::synthesize(BoolExprAST *ast) { return Type::Bool(); }
Type BiTypeCheckerVisitor::synthesize(VariableExprAST *ast) {
  return synthesize_id(ast->variableName);
}
Type BiTypeCheckerVisitor::synthesize(BlockAST *ast) { return Type::I64_t(); }
Type BiTypeCheckerVisitor::synthesize(IfExprAST *ast) {
  auto a = this->synthesize(ast->thenBlockAST.get());
  auto b = this->synthesize(ast->elseBlockAST.get());

  // TODO: something is not good here, we didn't check if a and b can converge.
  // TODO: Make a join function
  return a;
}
Type BiTypeCheckerVisitor::synthesize(TypedVarAST *ast) {
  return synthesize_typename(ast->type);
}
Type BiTypeCheckerVisitor::synthesize_id(const std::string &id) {
  if (this->get_id_map().queryName(id) == TypingContext::nameNotFound) {
  }
  return this->get_id_map().get_from_name(id);
}
Type BiTypeCheckerVisitor::synthesize_typename(const std::string &type_name) {
  return this->get_typename_map().get_from_name(type_name);
}

bool BiTypeCheckerVisitor::check(ProgramAST *ast) { return false; }
bool BiTypeCheckerVisitor::check(VarDefAST *ast) { return false; }
bool BiTypeCheckerVisitor::check(ExternAST *ast) { return false; }
bool BiTypeCheckerVisitor::check(FuncDefAST *ast) { return false; }
bool BiTypeCheckerVisitor::check(PrototypeAST *ast) { return false; }
bool BiTypeCheckerVisitor::check(CallExprAST *ast) { return false; }
bool BiTypeCheckerVisitor::check(BinaryExprAST *ast) { return false; }
bool BiTypeCheckerVisitor::check(NumberExprAST *ast) { return false; }
bool BiTypeCheckerVisitor::check(BoolExprAST *ast) { return false; }
bool BiTypeCheckerVisitor::check(VariableExprAST *ast) { return false; }
bool BiTypeCheckerVisitor::check(BlockAST *ast) { return false; }
bool BiTypeCheckerVisitor::check(IfExprAST *ast) { return false; }
bool BiTypeCheckerVisitor::check(TypedVarAST *ast) { return false; }
} // namespace sammine_lang::AST
