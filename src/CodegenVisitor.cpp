//
// Created by Jasmine Tang on 3/27/24.
//

#include "CodegenVisitor.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
namespace sammine_lang::AST {
void CgVisitor::visit(ProgramAST *ast) {
  for (auto &def : ast->DefinitionVec)
    def->accept_vis(this);
}
void CgVisitor::visit(VarDefAST *ast) {}
void CgVisitor::visit(PrototypeAST *ast) {
  std::vector<llvm::Type *> Doubles(
      ast->parameterVectors->size(),
      llvm::Type::getDoubleTy(*(resPtr->Context)));
  llvm::FunctionType *FT = llvm::FunctionType::get(
      llvm::Type::getDoubleTy(*resPtr->Context), Doubles, false);

  llvm::Function *F =
      llvm::Function::Create(FT, llvm::Function::ExternalLinkage,
                             ast->functionName, resPtr->Module.get());

  size_t param_index = 0;
  auto &vect = *ast->parameterVectors.get();
  for (auto &arg : F->args()) {
    auto &typed_var = vect[param_index++];
    arg.setName(typed_var->name);
  }
  ast->function = F;
}
void CgVisitor::visit(CallExprAST *ast) {}
void CgVisitor::visit(BinaryExprAST *ast) {
  ast->LHS->accept_vis(this);
  ast->RHS->accept_vis(this);

  auto L = ast->LHS->val;
  auto R = ast->RHS->val;

  if (ast->Op->type == TokenType::TokADD) {
    ast->val = resPtr->Builder->CreateFAdd(L, R, "addtmp");
  }
  if (ast->Op->type == TokenType::TokSUB) {
    ast->val = resPtr->Builder->CreateFSub(L, R, "subtmp");
  }
  if (ast->Op->type == TokenType::TokMUL) {
    ast->val = resPtr->Builder->CreateFMul(L, R, "multmp");
  }
  if (ast->Op->type == TokenType::TokDIV) {
    ast->val = resPtr->Builder->CreateFDiv(L, R, "multmp");
  }
  if (ast->Op->type == TokenType::TokLESS) {
    auto cmp_int = resPtr->Builder->CreateFCmpULT(L, R, "cmptmp");
    ast->val = resPtr->Builder->CreateUIToFP(
        L, llvm::Type::getDoubleTy(*(resPtr->Context)), "booltmp");
  }
}
void CgVisitor::visit(NumberExprAST *ast) {
  std::cerr << ast->number << std::endl;
  ast->val = llvm::ConstantFP::get(*resPtr->Context,
                                   llvm::APFloat(std::stod(ast->number)));
}
void CgVisitor::visit(VariableExprAST *ast) {
  ast->val = resPtr->NamedValues[ast->variableName];
}
void CgVisitor::visit(FuncDefAST *ast) {
  auto name = ast->Prototype->functionName;
  assert(resPtr);
  assert(resPtr->Module);
  assert(resPtr->Context);
  llvm::Function *Function = resPtr->Module->getFunction(name);

  if (!Function) {
    ast->Prototype->accept_vis(this);
    Function = ast->Prototype->function;
  }

  if (!Function) {
    // TODO: Please add better error handling
    std::cerr << "this should not happen" << std::endl;
    assert(false);
  }

  assert(Function);
  llvm::BasicBlock *mainblock =
      llvm::BasicBlock::Create(*resPtr->Context, "entry", Function);

  resPtr->Builder->SetInsertPoint(mainblock);

  resPtr->NamedValues.clear();
  for (auto &Arg : Function->args()) {
    resPtr->NamedValues[std::string(Arg.getName())] = &Arg;
  }

  std::cerr << "visiting block in funcdef ast " << std::endl;
  ast->Block->accept_vis(this);
  std::cerr << "visiting block after funcdef ast " << std::endl;

  // Validate the generated code, checking for consistency.
  auto verified = verifyFunction(*Function);
  // if (llvm::Value *RetVal = ast->Block->val) {
  //   // Finish off the function.
  // }

  // Error reading body, remove function.
  if (!verified) {
    Function->eraseFromParent();
  }
  return;
}

void CgVisitor::visit(BlockAST *ast) {
  for (auto &statement : ast->Statements) {
    statement->accept_vis(this);
  }
}
void CgVisitor::visit(TypedVarAST *ast) {};

} // namespace sammine_lang::AST
