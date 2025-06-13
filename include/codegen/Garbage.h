
#pragma once
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

#include "ast/Ast.h"
#include "ast/AstDecl.h"
#include <llvm/IR/GlobalVariable.h>
#include <memory>
#include <string>
#include <string_view>

namespace sammine_lang::AST {
class NumRootCalculator {
  int32_t calculate(BlockAST *ast);
  int32_t calculate(ExprAST *ast);
  int32_t calculate(IfExprAST *ast);
  int32_t calculate(VariableExprAST *ast);

  // TODO: Tell Jasmine to re-check this
  int32_t calculate(CallExprAST *ast);

  int32_t calculate(ReturnExprAST *ast);
  int32_t calculate(BinaryExprAST *ast);
  int32_t calculate(BoolExprAST *ast);
  int32_t calculate(StringExprAST *ast);
  int32_t calculate(NumberExprAST *ast);
  int32_t calculate(VarDefAST *ast);
};
class ShadowGarbageCollector {
  [[maybe_unused]]
  llvm::Module &module;
  llvm::LLVMContext &context;
  [[maybe_unused]]
  llvm::IRBuilder<> &builder;
  std::vector<llvm::Constant *> MetaDataEntries;

  // TODO: work with this in create frame map, rename the void to frame map
  // struct
  // rename to getFrameMap
  std::map<std::string, llvm::GlobalVariable *> fn_name_to_frame_map;
  llvm::StructType *createStackEntry(std::string_view);

public:
  ShadowGarbageCollector(llvm::Module &module, llvm::LLVMContext &context,
                         llvm::IRBuilder<> &builder)
      : module(module), context(context), builder(builder) {}

  virtual std::string llvmStrategy() { return "shadow-stack"; }
  void createFrameMapForCallee(FuncDefAST *);

  void setStackEntryFromCaller(FuncDefAST *);

  void initGlobalRootChain();

  void initGCFunc();
};
} // namespace sammine_lang::AST
