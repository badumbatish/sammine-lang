//
// Created by Jasmine Tang on 3/27/24.
//

#include "CodegenVisitor.h"
#include "Ast.h"
#include "Lexer.h"
#include "Utilities.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/raw_ostream.h"
#include <random>
namespace sammine_lang::AST {
using llvm::BasicBlock;

/// CreateEntryBlockAlloca - Create an alloca instruction in the entry block of
/// the function.  This is used for mutable variables etc.
llvm::AllocaInst *
CgVisitor::CreateEntryBlockAlloca(llvm::Function *Function,
                                  const std::string &VarName) {
  llvm::IRBuilder<> TmpB(&Function->getEntryBlock(),
                         Function->getEntryBlock().begin());
  return TmpB.CreateAlloca(llvm::Type::getDoubleTy(*resPtr->Context), nullptr,
                           VarName);
}
void CgVisitor::visit(ProgramAST *ast) {
  for (auto &def : ast->DefinitionVec)
    def->accept_vis(this);
}
void CgVisitor::visit(VarDefAST *ast) {}
void CgVisitor::visit(PrototypeAST *ast) {
  std::vector<llvm::Type *> Doubles(
      ast->parameterVectors.size(),
      llvm::Type::getDoubleTy(*(resPtr->Context)));
  llvm::FunctionType *FT = llvm::FunctionType::get(
      llvm::Type::getDoubleTy(*resPtr->Context), Doubles, false);

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
}
void CgVisitor::visit(CallExprAST *ast) {
  llvm::Function *callee = resPtr->Module->getFunction(ast->functionName);
  if (!callee)
    sammine_util::abort("Unknown function called");

  if (ast->arguments.size() != callee->arg_size())
    sammine_util::abort("Incorrect number of arguments passed");
  std::vector<llvm::Value *> ArgsVector;

  for (size_t i = 0; i < ast->arguments.size(); i++) {
    auto arg_ast = ast->arguments[i].get();
    arg_ast->accept_vis(this);
    ArgsVector.push_back(arg_ast->val);
  }

  ast->val = resPtr->Builder->CreateCall(callee, ArgsVector, "calltmp");
}
void CgVisitor::visit(BinaryExprAST *ast) {
  if (ast->Op->type == TokenType::TokASSIGN) {
    VariableExprAST *LHSE = static_cast<VariableExprAST *>(ast->LHS.get());
    if (!LHSE)
      sammine_util::abort("Left hand side of assignment must be a variable");

    ast->RHS->accept_vis(this);
    auto R = ast->RHS->val;

    if (!R)
      sammine_util::abort("Failed to codegen RHS for tok assign");

    auto *Var = resPtr->NamedValues[LHSE->variableName];
    if (!Var)
      sammine_util::abort("Unknown variable in LHS of tok assign");

    resPtr->Builder->CreateStore(R, Var);
    ast->val = R;
    return;
  }
  ast->LHS->accept_vis(this);
  ast->RHS->accept_vis(this);

  auto L = ast->LHS->val;
  auto R = ast->RHS->val;

  if (ast->Op->type == TokenType::TokADD) {
    ast->val = resPtr->Builder->CreateFAdd(L, R, "add_expr");
  }
  if (ast->Op->type == TokenType::TokSUB) {
    ast->val = resPtr->Builder->CreateFSub(L, R, "sub_expr");
  }
  if (ast->Op->type == TokenType::TokMUL) {
    ast->val = resPtr->Builder->CreateFMul(L, R, "mul_expr");
  }
  if (ast->Op->type == TokenType::TokDIV) {
    ast->val = resPtr->Builder->CreateFDiv(L, R, "div_expr");
  }
  if (ast->Op->type == TokenType::TokLESS) {
    /*auto cmp_int = resPtr->Builder->CreateFCmpULT(L, R, "less_cmp_expr");*/
    ast->val = resPtr->Builder->CreateUIToFP(
        L, llvm::Type::getDoubleTy(*(resPtr->Context)), "bool_expr");
  }
}
void CgVisitor::visit(BoolExprAST *ast) {
  if (ast->b)
    ast->val = llvm::ConstantFP::get(*resPtr->Context,
                                     llvm::APFloat(std::stod("1.0")));
  else
    ast->val = llvm::ConstantFP::get(*resPtr->Context,
                                     llvm::APFloat(std::stod("0.0")));
}
void CgVisitor::visit(IfExprAST *ast) {
  ast->bool_expr->accept_vis(this);
  if (!ast->bool_expr->val) {
    sammine_util::abort("Failed to codegen condition of if-expr");
  }
  ast->bool_expr->val = resPtr->Builder->CreateFCmpONE(
      ast->bool_expr->val,
      llvm::ConstantFP::get(*resPtr->Context, llvm::APFloat(0.0)), "ifcond");

  llvm::Function *function = resPtr->Builder->GetInsertBlock()->getParent();

  // Create blocks for the then and else cases.  Insert the 'then' block at the
  // end of the function.
  BasicBlock *ThenBB = BasicBlock::Create(*resPtr->Context, "then", function);
  BasicBlock *ElseBB = BasicBlock::Create(*resPtr->Context, "else");
  BasicBlock *MergeBB = BasicBlock::Create(*resPtr->Context, "ifcont");

  resPtr->Builder->CreateCondBr(ast->bool_expr->val, ThenBB, ElseBB);

  std::cerr << "Hi from if expr codegen" << std::endl;

  resPtr->Builder->SetInsertPoint(ThenBB);

  ast->thenBlockAST->accept_vis(this);
  if (!ast->thenBlockAST->val) {
    sammine_util::abort("Failed to generate then block for if-expr");
    return;
  }

  resPtr->Builder->CreateBr(MergeBB);

  ThenBB = resPtr->Builder->GetInsertBlock();

  function->insert(function->end(), ElseBB);
  resPtr->Builder->SetInsertPoint(ElseBB);

  ast->elseBlockAST->accept_vis(this);
  if (!ast->elseBlockAST->val) {
    return;
  }

  resPtr->Builder->CreateBr(MergeBB);

  ElseBB = resPtr->Builder->GetInsertBlock();
  function->insert(function->end(), MergeBB);
  resPtr->Builder->SetInsertPoint(MergeBB);
  llvm::PHINode *PN = resPtr->Builder->CreatePHI(
      llvm::Type::getDoubleTy(*resPtr->Context), 2, "iftmp");

  PN->addIncoming(ast->thenBlockAST->val, ThenBB);
  PN->addIncoming(ast->elseBlockAST->val, ElseBB);
  ast->val = PN;
}
void CgVisitor::visit(NumberExprAST *ast) {
  ast->val = llvm::ConstantFP::get(*resPtr->Context,
                                   llvm::APFloat(std::stod(ast->number)));
}
void CgVisitor::visit(VariableExprAST *ast) {
  auto *Alloca = resPtr->NamedValues[ast->variableName];
  if (!Alloca)
    sammine_util::abort("Unknown variable name");

  ast->val = resPtr->Builder->CreateLoad(Alloca->getAllocatedType(), Alloca,
                                         ast->variableName);
}
void CgVisitor::visit(ExternAST *ast) {

  ast->Prototype->accept_vis(this);

  resPtr->FnProto[ast->Prototype->functionName] = std::move(ast->Prototype);
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
    assert(false);
    sammine_util::abort("This should not happen");
  }

  assert(Function);
  llvm::BasicBlock *mainblock =
      llvm::BasicBlock::Create(*resPtr->Context, "entry", Function);

  resPtr->Builder->SetInsertPoint(mainblock);

  resPtr->NamedValues.clear();
  for (auto &Arg : Function->args()) {
    llvm::AllocaInst *Alloca =
        CreateEntryBlockAlloca(Function, std::string(Arg.getName()));
    resPtr->Builder->CreateStore(&Arg, Alloca);
    resPtr->NamedValues[std::string(Arg.getName())] = Alloca;
  }

  ast->Block->accept_vis(this);

  // TODO: A function should return the last expression (only float for now)
  resPtr->Builder->CreateRet(
      llvm::ConstantFP::get(*resPtr->Context, llvm::APFloat(1.0)));
  // Validate the generated code, checking for consistency.
  auto not_verified = verifyFunction(*Function, &llvm::errs());
  // if (llvm::Value *RetVal = ast->Block->val) {
  //   // Finish off the function.
  // }

  // Error reading body, remove function.
  if (not_verified) {
    sammine_util::abort("ICE: Abort from creating a function");
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
