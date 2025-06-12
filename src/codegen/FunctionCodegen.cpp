
#include "codegen/CodegenVisitor.h"

#include "ast/Ast.h"
#include "fmt/format.h"
#include "util/Utilities.h"
#include <cstdint>
namespace sammine_lang::AST {

// int32_t calculate_num_roots(BlockAST *ast) {
//   int32_t num_roots = 0;
//   for (auto &stmt : ast->Statements) {
//
//   }
// }

class NumRootCalculator {

  int32_t calculate(BlockAST *ast) {
    int num_roots = 0;
    for (auto &e : ast->Statements) {
      num_roots += calculate(e.get());
    }

    return num_roots;
  }
  int32_t calculate(ExprAST *ast) {
    if (auto e = dynamic_cast<IfExprAST *>(ast)) {
      return calculate(e);
    } else if (auto e = dynamic_cast<VariableExprAST *>(ast)) {
      return calculate(e);
    } else if (auto e = dynamic_cast<CallExprAST *>(ast)) {
      return calculate(e);
    } else if (auto e = dynamic_cast<ReturnExprAST *>(ast)) {
      return calculate(e);
    } else if (auto e = dynamic_cast<BoolExprAST *>(ast)) {
      return calculate(e);
    } else if (auto e = dynamic_cast<StringExprAST *>(ast)) {
      return calculate(e);
    } else if (auto e = dynamic_cast<NumberExprAST *>(ast)) {
      return calculate(e);
    } else if (auto e = dynamic_cast<VarDefAST *>(ast)) {
      return calculate(e);
    } else {
      sammine_util::abort(
          fmt::format("You should be overloading on {}", ast->getTreeName()));
      return 0;
    }
  }
  int32_t calculate(IfExprAST *ast) {
    return calculate(ast->bool_expr.get()) +
           calculate(ast->thenBlockAST.get()) +
           calculate(ast->elseBlockAST.get());
  }
  int32_t calculate(VariableExprAST *ast) { return 0; }

  // TODO: Tell Jasmine to re-check this
  int32_t calculate(CallExprAST *ast) {
    int32_t num_roots = 0;
    for (auto &arg : ast->arguments) {
      num_roots += calculate(arg.get());
    }
    return num_roots;
  }

  int32_t calculate(ReturnExprAST *ast) {
    return calculate(ast->return_expr.get());
  }
  int32_t calculate(BinaryExprAST *ast) {
    auto left = calculate(ast->LHS.get());
    auto right = calculate(ast->RHS.get());
    return left + right;
  }
  int32_t calculate(BoolExprAST *ast) { return 0; }
  int32_t calculate(StringExprAST *ast) { return 1; }
  int32_t calculate(NumberExprAST *ast) { return 0; }
  int32_t calculate(VarDefAST *ast) { return calculate(ast->Expression.get()); }
};
void CgVisitor::preorder_walk(BlockAST *ast) {}
void CgVisitor::postorder_walk(BlockAST *ast) {}
/// INFO: Register the function with its arguments, put it in the module
/// this comes before visiting a function
void CgVisitor::preorder_walk(PrototypeAST *ast) {
  std::vector<llvm::Type *> param_types;
  for (auto &p : ast->parameterVectors) {
    param_types.push_back(type_converter.get_type(p->type));
  }
  llvm::FunctionType *FT = llvm::FunctionType::get(
      this->type_converter.get_return_type(ast->type), param_types, false);
  llvm::Function *F =
      llvm::Function::Create(FT, llvm::Function::ExternalLinkage,
                             ast->functionName, resPtr->Module.get());

  size_t param_index = 0;
  auto &vect = ast->parameterVectors;
  for (auto &arg : F->args()) {
    auto &typed_var = vect[param_index++];
    arg.setName(typed_var->name);
  }
  ast->function = F;
  current_func = F;
  assert(F);
}

void CgVisitor::postorder_walk(FuncDefAST *ast) {
  // TODO: A function should return the last expression (only float for now)
  auto not_verified = verifyFunction(*getCurrentFunction(), &llvm::errs());
  // if (llvm::Value *RetVal = ast->Block->val) {
  //   // Finish off the function.
  // }

  // Error reading body, remove function.
  if (not_verified) {
    resPtr->Module->print(llvm::errs(), nullptr);
    this->abort("ICE: Abort from creating a function");
    getCurrentFunction()->eraseFromParent();
  }
}

void CgVisitor::preorder_walk(CallExprAST *ast) {

  llvm::Function *callee = resPtr->Module->getFunction(ast->functionName);
  if (!callee) {
    this->abort("Unknown function called");
    return;
  }

  if (ast->arguments.size() != callee->arg_size())
    this->abort("Incorrect number of arguments passed");
  std::vector<llvm::Value *> ArgsVector;

  for (size_t i = 0; i < ast->arguments.size(); i++) {
    auto arg_ast = ast->arguments[i].get();
    arg_ast->accept_vis(this);
    ArgsVector.push_back(arg_ast->val);
  }

  ast->val = resPtr->Builder->CreateCall(callee, ArgsVector, "calltmp");
}
} // namespace sammine_lang::AST
