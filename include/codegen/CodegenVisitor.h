//
// Created by Jasmine Tang on 3/27/24.
//

#pragma once
#include "ast/Ast.h"
#include "ast/AstBase.h"
#include "codegen/LLVMRes.h"
#include "util/LexicalContext.h"
#include <llvm/IR/Function.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <memory>
#include <utility>
#include <vector>
namespace sammine_lang::AST {
class TypeConverter {

  llvm::LLVMContext *context;

public:
  llvm::Type *get_type(Type t) {
    switch (t.type_kind) {
    case TypeKind::I64_t:
      return llvm::Type::getInt64Ty(*context);
    case TypeKind::F64_t:
      return llvm::Type::getDoubleTy(*context);
    case TypeKind::Unit:
      return llvm::Type::getVoidTy(*context);
    case TypeKind::Bool:
      return llvm::Type::getInt1Ty(*context);
    case TypeKind::Function:
      sammine_util::abort("Function is not first-class yet");
    case TypeKind::NonExistent:
      sammine_util::abort("Existed a type that is not synthesized yet");
    case TypeKind::Poisoned:
      sammine_util::abort("Poisoned typed should not be here");
      break;
    }
    sammine_util::abort("Guarded by default case");
  }
  llvm::Type *get_return_type(Type t) {

    switch (t.type_kind) {
    case TypeKind::Function:
      return get_type(t.type_data->get_return_type());
    default:
      sammine_util::abort(
          "Jasmine passed in something that is not a function type");
      break;
    }
    sammine_util::abort("Guarded by default case");
  }
  llvm::CmpInst::Predicate get_cmp_func(Type a, Type b, TokenType tok) {
    sammine_util::abort_if_not(a.type_kind == b.type_kind,
                               "Two types needs to be the same");
    using llvm::CmpInst;

    switch (a.type_kind) {

    case TypeKind::I64_t:
    case TypeKind::Bool: {
      // Signed integer comparisons
      switch (tok) {
      case TokenType::TokEQUAL:
        return CmpInst::ICMP_EQ;
      // case TokenType::TokNOTEqual:
      //   return CmpInst::ICMP_ONE;
      case TokenType::TokLESS:
        return CmpInst::ICMP_SLT;
      case TokenType::TokLessEqual:
        return CmpInst::ICMP_SLE;
      case TokenType::TokGREATER:
        return CmpInst::ICMP_SGT;
      case TokenType::TokGreaterEqual:
        return CmpInst::ICMP_SGE;
      default:

        sammine_util::abort("Invalid token for integer comparison");
      }
      break;
    }
    case TypeKind::F64_t: {
      // Ordered floating-point comparisons
      switch (tok) {
      case TokenType::TokEQUAL:
        return CmpInst::FCMP_OEQ;
      // case TokenType::TokNOTEqual:
      //   return CmpInst::FCMP_ONE;
      case TokenType::TokLESS:
        return CmpInst::FCMP_OLT;
      case TokenType::TokLessEqual:
        return CmpInst::FCMP_OLE;
      case TokenType::TokGREATER:
        return CmpInst::FCMP_OGT;
      case TokenType::TokGreaterEqual:
        return CmpInst::FCMP_OGE;
      default:
        sammine_util::abort("Invalid token for float comparison");
      }
      break;
    }
    case TypeKind::Unit:
      sammine_util::abort("Cannot compare values of this type");
    case TypeKind::Function:
      sammine_util::abort("Cannot compare values of this type");
    case TypeKind::NonExistent:
      sammine_util::abort("Cannot compare values of this type");
    case TypeKind::Poisoned:
      sammine_util::abort("Cannot compare values of this type");
      break;
    }
    sammine_util::abort("End of get_cmp_func reached");
  }

  TypeConverter(llvm::LLVMContext *context) : context(context) {}
};
class CgVisitor : public ScopedASTVisitor {

private:
  std::shared_ptr<sammine_lang::LLVMRes> resPtr;
  std::stack<std::map<std::string, llvm::AllocaInst *>> allocaValues;

  llvm::Function *current_func;
  llvm::Function *getCurrentFunction();
  void setCurrentFunction(std::shared_ptr<PrototypeAST>);

  TypeConverter type_converter;

public:
  CgVisitor(std::shared_ptr<sammine_lang::LLVMRes> resPtr)
      : resPtr(resPtr), type_converter(resPtr->Context.get()) {
    assert(this->resPtr);
  }
  llvm::AllocaInst *CreateEntryBlockAlloca(llvm::Function *TheFunction,
                                           const std::string &VarName,
                                           llvm::Type *);

  void enter_new_scope() override {
    allocaValues.push(std::map<std::string, llvm::AllocaInst *>());
  }
  void exit_new_scope() override { allocaValues.pop(); }

  virtual void visit(FuncDefAST *) override;
  virtual void visit(IfExprAST *ast) override {
    ast->walk_with_preorder(this);
    ast->walk_with_postorder(this);
  }
  // visit
  // pre order
  // TODO: Implement these
  virtual void preorder_walk(ProgramAST *ast) override;
  virtual void preorder_walk(VarDefAST *ast) override;
  virtual void preorder_walk(FuncDefAST *ast) override;
  virtual void preorder_walk(ExternAST *ast) override {}
  virtual void preorder_walk(PrototypeAST *ast) override;
  virtual void preorder_walk(CallExprAST *ast) override;
  virtual void preorder_walk(ReturnExprAST *ast) override {}
  virtual void preorder_walk(BinaryExprAST *ast) override;
  virtual void preorder_walk(NumberExprAST *ast) override;
  virtual void preorder_walk(BoolExprAST *ast) override;
  virtual void preorder_walk(VariableExprAST *ast) override;
  virtual void preorder_walk(BlockAST *ast) override;
  virtual void preorder_walk(IfExprAST *ast) override;
  virtual void preorder_walk(TypedVarAST *ast) override;

  // post order
  // TODO: Implement these?
  virtual void postorder_walk(ProgramAST *ast) override {}
  virtual void postorder_walk(VarDefAST *ast) override;
  virtual void postorder_walk(ExternAST *ast) override {}
  virtual void postorder_walk(FuncDefAST *ast) override;
  virtual void postorder_walk(PrototypeAST *ast) override {}
  virtual void postorder_walk(CallExprAST *ast) override {}
  virtual void postorder_walk(ReturnExprAST *ast) override;
  virtual void postorder_walk(BinaryExprAST *ast) override;
  virtual void postorder_walk(NumberExprAST *ast) override {}
  virtual void postorder_walk(BoolExprAST *ast) override {}
  virtual void postorder_walk(VariableExprAST *ast) override {}
  virtual void postorder_walk(BlockAST *ast) override;
  virtual void postorder_walk(IfExprAST *ast) override {}
  virtual void postorder_walk(TypedVarAST *ast) override {}
};
} // namespace sammine_lang::AST
