//
// Created by Jasmine Tang on 3/27/24.
//

#pragma once
#include "Ast.h"
#include "AstBase.h"
#include "LLVMRes.h"
#include "LexicalContext.h"
#include <llvm/IR/Instructions.h>
#include <memory>
namespace sammine_lang::AST {
class CgVisitor : public ScopedASTVisitor {
  using AllocaContext = LexicalContext<llvm::AllocaInst *>;
  using fnProtoContext = LexicalContext<std::shared_ptr<PrototypeAST>>;

private:
  std::shared_ptr<sammine_lang::LLVMRes> resPtr;
  std::stack<AllocaContext> namedValues;
  std::stack<fnProtoContext> fnProto;

public:
  CgVisitor(std::shared_ptr<sammine_lang::LLVMRes> resPtr) : resPtr(resPtr) {
    assert(this->resPtr);

    namedValues.push(AllocaContext());
    fnProto.push(fnProtoContext());
  }
  llvm::AllocaInst *CreateEntryBlockAlloca(llvm::Function *TheFunction,
                                           const std::string &VarName);

  void enter_new_scope() override {
    namedValues.push(AllocaContext(&namedValues.top()));
    fnProto.push(fnProtoContext(&fnProto.top()));
  }
  void exit_new_scope() override {
    namedValues.pop();
    fnProto.pop();
  }
  // visit
  // pre order
  // TODO: Implement these
  virtual void preorder_walk(ProgramAST *ast) override;
  virtual void preorder_walk(VarDefAST *ast) override;
  virtual void preorder_walk(ExternAST *ast) override;
  virtual void preorder_walk(FuncDefAST *ast) override;
  virtual void preorder_walk(PrototypeAST *ast) override;
  virtual void preorder_walk(CallExprAST *ast) override;
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
  virtual void postorder_walk(VarDefAST *ast) override {}
  virtual void postorder_walk(ExternAST *ast) override;
  virtual void postorder_walk(FuncDefAST *ast) override {}
  virtual void postorder_walk(PrototypeAST *ast) override {}
  virtual void postorder_walk(CallExprAST *ast) override {}
  virtual void postorder_walk(BinaryExprAST *ast) override {}
  virtual void postorder_walk(NumberExprAST *ast) override {}
  virtual void postorder_walk(BoolExprAST *ast) override {}
  virtual void postorder_walk(VariableExprAST *ast) override {}
  virtual void postorder_walk(BlockAST *ast) override {}
  virtual void postorder_walk(IfExprAST *ast) override {}
  virtual void postorder_walk(TypedVarAST *ast) override {}
};
} // namespace sammine_lang::AST
