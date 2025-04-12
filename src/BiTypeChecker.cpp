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

  // Case 1: let x = a //
  // Case 2: let x : f64_t = a //
  //
  //
  // Case 1
  if (ast->TypedVar->accept_synthesis(this) == Type::NonExistent()) {
  }
  sammine_util::abort("Yes, failing right now");
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
  return Type::NonExistent();
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
  return Type::NonExistent();
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
  return Type::NonExistent();
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
