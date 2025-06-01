//
// Created by Jasmine Tang on 3/27/24.
//

#pragma once
#include "Ast.h"
#include "AstBase.h"
#include "LLVMRes.h"
#include "LexicalContext.h"
#include "Utilities.h"
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <memory>
#include <utility>
#include <vector>
namespace sammine_lang::AST {
class BlockValues {
  std::map<BlockAST *, std::vector<std::pair<std::string, llvm::Value *>>>
      block_to_values;
  std::stack<std::vector<std::pair<std::string, llvm::Value *>> *>
      current_block_values;

public:
  void push_new_block(BlockAST *ast) {
    current_block_values.push(&block_to_values[ast]);
  }
  void pop() { current_block_values.pop(); }

  void register_assignment(const std::string &name, llvm::Value *val) {
    current_block_values.top()->push_back({name, val});
  }

  std::map<std::string, std::pair<llvm::Value *, llvm::Value *>>
  get_similarity(BlockAST *a, BlockAST *b) {
    // INFO: get all names of *a in a set
    // if a name in b matches, put it in
    std::vector<std::pair<llvm::Value *, llvm::Value *>> res;

    std::map<std::string, std::pair<llvm::Value *, llvm::Value *>>
        names_to_pair_values;
    for (auto [name, val] : block_to_values[a])
      names_to_pair_values[name].first = val;

    for (auto [name, val] : block_to_values[b]) {
      if (names_to_pair_values.contains(name)) {
        names_to_pair_values[name].second = val;
      }
    }

    return names_to_pair_values;
  }
};
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
      sammine_util::abort("Jas hasnt figure out yet how to do this");
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
  TypeConverter(llvm::LLVMContext *context) : context(context) {}
};
class CgVisitor : public ScopedASTVisitor {

private:
  std::shared_ptr<sammine_lang::LLVMRes> resPtr;
  std::stack<std::map<std::string, llvm::AllocaInst *>> allocaValues;

  llvm::Function *current_func;
  llvm::Function *getCurrentFunction();
  void setCurrentFunction(std::shared_ptr<PrototypeAST>);
  BlockValues bv;

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
