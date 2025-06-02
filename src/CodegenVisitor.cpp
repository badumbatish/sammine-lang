//
// Created by Jasmine Tang on 3/27/24.
//

#include "CodegenVisitor.h"
#include "Ast.h"
#include "Lexer.h"
#include "Types.h"
#include "Utilities.h"
#include "fmt/format.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/raw_ostream.h"
#include <cstdlib>
#include <random>
#include <ranges>
#include <utility>
namespace sammine_lang::AST {
using llvm::BasicBlock;

/// CreateEntryBlockAlloca - Create an alloca instruction in the entry block of
/// the function.  This is used for mutable variables etc.
///
/// from the llvm blog:
/// mem2reg only looks for alloca instructions in the entry block of the
/// function. Being in the entry block guarantees that the alloca is only
/// executed once, which makes analysis simpler.
llvm::AllocaInst *CgVisitor::CreateEntryBlockAlloca(llvm::Function *Function,
                                                    const std::string &VarName,
                                                    llvm::Type *type) {
  llvm::IRBuilder<> TmpB(&Function->getEntryBlock(),
                         Function->getEntryBlock().begin());
  return TmpB.CreateAlloca(type, nullptr, VarName);
}

llvm::Function *CgVisitor::getCurrentFunction() { return this->current_func; }

void CgVisitor::setCurrentFunction(std::shared_ptr<PrototypeAST> ptr) {

  sammine_util::abort_if_not(ptr,
                             "A shared ptr cannot be null at this point in "
                             "codegen, something is wrong with your parsing.");

  this->current_func = ptr->function;
}
void CgVisitor::visit(FuncDefAST *ast) {

  this->enter_new_scope();
  ast->Prototype->accept_vis(this);
  ast->walk_with_preorder(this);
  ast->Block->accept_vis(this);
  ast->walk_with_postorder(this);
  this->exit_new_scope();
}
void CgVisitor::preorder_walk(ProgramAST *ast) {}

void CgVisitor::preorder_walk(VarDefAST *ast) {
  auto var_name = ast->TypedVar->name;
  auto alloca = this->CreateEntryBlockAlloca(
      getCurrentFunction(), var_name, type_converter.get_type(ast->type));
  this->allocaValues.top()[var_name] = alloca;
}
void CgVisitor::postorder_walk(VarDefAST *ast) {
  auto var_name = ast->TypedVar->name;
  auto alloca = this->allocaValues.top()[var_name];
  if (ast->Expression == nullptr) {
    sammine_util::abort_if_not(ast->Expression, "is this legal?");
  } else {
    resPtr->Builder->CreateStore(ast->Expression->val, alloca);
  }
}
void CgVisitor::preorder_walk(FuncDefAST *ast) {
  auto name = ast->Prototype->functionName;
  sammine_util::abort_if_not(resPtr);
  sammine_util::abort_if_not(resPtr->Module);
  sammine_util::abort_if_not(resPtr->Context);

  auto *Function = this->getCurrentFunction();

  assert(Function);
  llvm::BasicBlock *mainblock =
      llvm::BasicBlock::Create(*(resPtr->Context), "entry", Function);

  resPtr->Builder->SetInsertPoint(mainblock);

  //
  // INFO:: Copy all the arguments to the entry block
  for (auto &Arg : Function->args()) {
    llvm::AllocaInst *Alloca = CreateEntryBlockAlloca(
        Function, std::string(Arg.getName()), Arg.getType());
    resPtr->Builder->CreateStore(&Arg, Alloca);

    this->allocaValues.top()[std::string(Arg.getName())] = Alloca;
  }
  return;
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
    sammine_util::abort("ICE: Abort from creating a function");
    getCurrentFunction()->eraseFromParent();
  }
}

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
void CgVisitor::preorder_walk(CallExprAST *ast) {

  llvm::Function *callee = resPtr->Module->getFunction(ast->functionName);
  if (!callee) {
    sammine_util::abort("Unknown function called");
    return;
  }
  if (callee->getReturnType() == llvm::Type::getVoidTy(*resPtr->Context)) {
    std::cout << "uhhh" << std::endl;
  }

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

void CgVisitor::postorder_walk(ReturnExprAST *ast) {
  // INFO: If we cannot parse return expr, treat it as unit for now
  if (!ast->return_expr || ast->return_expr->type == Type::Unit())
    resPtr->Builder->CreateRetVoid();
  else
    resPtr->Builder->CreateRet(ast->return_expr->val);
}

void CgVisitor::preorder_walk(BinaryExprAST *ast) {}

void CgVisitor::postorder_walk(BinaryExprAST *ast) {
  if (ast->Op->tok_type == TokenType::TokASSIGN) {
    VariableExprAST *LHSE = static_cast<VariableExprAST *>(ast->LHS.get());
    if (!LHSE) {
      sammine_util::abort("Left hand side of assignment must be a variable, "
                          "current LHS cannot "
                          "be statically cast to VarExprAST");
      return;
    }

    auto R = ast->RHS->val;

    if (!R)
      sammine_util::abort("Failed to codegen RHS for tok assign");

    auto *Var = this->allocaValues.top()[LHSE->variableName];
    if (!Var)
      sammine_util::abort("Unknown variable in LHS of tok assign");

    resPtr->Builder->CreateStore(R, Var);
    ast->val = R;

    return;
  }
  auto L = ast->LHS->val;
  auto R = ast->RHS->val;

  if (ast->Op->tok_type == TokenType::TokADD) {
    if (ast->LHS->type == Type::I64_t())
      ast->val = resPtr->Builder->CreateAdd(L, R, "add_expr");
    else if (ast->LHS->type == Type::F64_t())
      ast->val = resPtr->Builder->CreateFAdd(L, R, "add_expr");
    else
      sammine_util::abort();
  }
  if (ast->Op->tok_type == TokenType::TokSUB) {
    if (ast->LHS->type == Type::I64_t())
      ast->val = resPtr->Builder->CreateSub(L, R, "sub_expr");
    else if (ast->LHS->type == Type::F64_t())
      ast->val = resPtr->Builder->CreateFSub(L, R, "sub_expr");
    else
      sammine_util::abort();
  }
  if (ast->Op->tok_type == TokenType::TokMUL) {
    if (ast->LHS->type == Type::I64_t())
      ast->val = resPtr->Builder->CreateMul(L, R, "mul_expr");
    else if (ast->LHS->type == Type::F64_t())
      ast->val = resPtr->Builder->CreateFMul(L, R, "mul_expr");
    else
      sammine_util::abort();
  }
  if (ast->Op->tok_type == TokenType::TokDIV) {
    if (ast->LHS->type == Type::F64_t())
      ast->val = resPtr->Builder->CreateFDiv(L, R, "div_expr");
    else
      sammine_util::abort();
  }
  if (ast->Op->tok_type == TokOR) {
    ast->val = resPtr->Builder->CreateLogicalOr(ast->LHS->val, ast->RHS->val);
  }
  if (ast->Op->tok_type == TokAND) {
    ast->val = resPtr->Builder->CreateLogicalAnd(ast->LHS->val, ast->RHS->val);
  }
  if (ast->Op->is_comparison()) {
    /*auto cmp_int = resPtr->Builder->CreateFCmpULT(L, R, "less_cmp_expr");*/
    ast->val = resPtr->Builder->CreateCmp(
        type_converter.get_cmp_func(ast->LHS->type, ast->RHS->type,
                                    ast->Op->tok_type),
        L, R);
  }
  if (!ast->val) {
    std::cout << ast->Op->lexeme << std::endl;
    sammine_util::abort();
  }

  return;
}

void CgVisitor::preorder_walk(NumberExprAST *ast) {
  switch (ast->type.type_kind) {
  case TypeKind::I64_t:
    ast->val = llvm::ConstantInt::get(
        *resPtr->Context, llvm::APInt(64, std::stoi(ast->number), true));
    break;
  case TypeKind::F64_t:
    ast->val = llvm::ConstantFP::get(*resPtr->Context,
                                     llvm::APFloat(std::stod(ast->number)));
    break;
  case TypeKind::Unit:
  case TypeKind::Bool:
  case TypeKind::Function:
  case TypeKind::NonExistent:
  case TypeKind::Poisoned:
    sammine_util::abort(".....");
    break;
  }
  sammine_util::abort_if_not(ast->val, "cannot generate number");
}
void CgVisitor::preorder_walk(BoolExprAST *ast) {
  if (ast->b)
    ast->val = llvm::ConstantFP::get(*resPtr->Context,
                                     llvm::APFloat(std::stod("1.0")));
  else
    ast->val = llvm::ConstantFP::get(*resPtr->Context,
                                     llvm::APFloat(std::stod("0.0")));
}
void CgVisitor::preorder_walk(VariableExprAST *ast) {
  auto *alloca = this->allocaValues.top()[ast->variableName];

  sammine_util::abort_if_not(alloca, "Unknown variable name");

  ast->val = resPtr->Builder->CreateLoad(alloca->getAllocatedType(), alloca,
                                         ast->variableName);
}
void CgVisitor::preorder_walk(BlockAST *ast) {}
void CgVisitor::postorder_walk(BlockAST *ast) {}
void CgVisitor::preorder_walk(IfExprAST *ast) {
  ast->bool_expr->accept_vis(this);
  if (!ast->bool_expr->val) {
    sammine_util::abort("Failed to codegen condition of if-expr");
  }
  switch (ast->bool_expr->type.type_kind) {
  case TypeKind::I64_t:
    ast->bool_expr->val = resPtr->Builder->CreateFCmpONE(
        resPtr->Builder->CreateSIToFP(
            ast->bool_expr->val, llvm::Type::getDoubleTy(*resPtr->Context)),
        llvm::ConstantFP::get(*resPtr->Context, llvm::APFloat(0.0)),
        "ifcond_i64");
    break;
  case TypeKind::F64_t:
    ast->bool_expr->val = resPtr->Builder->CreateFCmpONE(
        ast->bool_expr->val,
        llvm::ConstantFP::get(*resPtr->Context, llvm::APFloat(0.0)),
        "ifcond_f64");
    break;
  case TypeKind::NonExistent:
    ASTPrinter::print(ast->bool_expr.get());
    sammine_util::abort("Invalid syntax for now");
    break;
  case TypeKind::Poisoned:
    sammine_util::abort("Invalid syntax for now");
    break;
  case TypeKind::Unit:
    sammine_util::abort("Invalid syntax for now");
    break;
  case TypeKind::Bool:
    ast->bool_expr->val = resPtr->Builder->CreateFCmpONE(
        resPtr->Builder->CreateUIToFP(
            ast->bool_expr->val, llvm::Type::getDoubleTy(*resPtr->Context)),
        llvm::ConstantFP::get(*resPtr->Context, llvm::APFloat(0.0)),
        "ifcond_bool");
    break;
  case TypeKind::Function:
    sammine_util::abort("Invalid syntax for now");
    break;
  }

  llvm::Function *function = resPtr->Builder->GetInsertBlock()->getParent();

  // Create blocks for the then and else cases.  Insert the 'then' block at
  // the end of the function.
  BasicBlock *ThenBB = BasicBlock::Create(*resPtr->Context, "then", function);
  BasicBlock *ElseBB = BasicBlock::Create(*resPtr->Context, "else");
  BasicBlock *MergeBB = BasicBlock::Create(*resPtr->Context, "ifcont");

  resPtr->Builder->CreateCondBr(ast->bool_expr->val, ThenBB, ElseBB);

  resPtr->Builder->SetInsertPoint(ThenBB);

  ast->thenBlockAST->accept_vis(this);
  if (!ThenBB->back().isTerminator())
    resPtr->Builder->CreateBr(MergeBB);

  function->insert(function->end(), ElseBB);
  resPtr->Builder->SetInsertPoint(ElseBB);

  ast->elseBlockAST->accept_vis(this);

  if (!ElseBB->back().isTerminator())
    resPtr->Builder->CreateBr(MergeBB);

  function->insert(function->end(), MergeBB);
  resPtr->Builder->SetInsertPoint(MergeBB);
}
void CgVisitor::preorder_walk(TypedVarAST *ast) {}

} // namespace sammine_lang::AST
