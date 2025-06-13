
#include "ast/Ast.h"
#include "codegen/CodegenVisitor.h"
namespace sammine_lang::AST {

void CgVisitor::preorder_walk(BlockAST *ast) {}
void CgVisitor::postorder_walk(BlockAST *ast) {}
/// INFO: Register the function with its arguments, put it in the module
/// this comes before visiting a function
void CgVisitor::preorder_walk(PrototypeAST *ast) {
  std::vector<llvm::Type *> param_types;
  for (auto &p : ast->parameterVectors)
    param_types.push_back(type_converter.get_type(p->type));

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
