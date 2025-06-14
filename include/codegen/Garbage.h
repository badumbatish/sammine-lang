
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
public:
  static int32_t calculate(BlockAST *ast);
  static int32_t calculate(ExprAST *ast);
  static int32_t calculate(IfExprAST *ast);
  static int32_t calculate(VariableExprAST *ast);

  // TODO: Tell Jasmine to re-check this
  static int32_t calculate(CallExprAST *ast);

  static int32_t calculate(ReturnExprAST *ast);
  static int32_t calculate(BinaryExprAST *ast);
  static int32_t calculate(BoolExprAST *ast);
  static int32_t calculate(StringExprAST *ast);
  static int32_t calculate(NumberExprAST *ast);
  static int32_t calculate(UnitExprAST *ast);
  static int32_t calculate(VarDefAST *ast);
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
  llvm::StructType *GetStackEntryType();
  inline static std::string GLOBAL_ROOT_CHAIN = "global_root_chain";

public:
  ShadowGarbageCollector(llvm::Module &module, llvm::LLVMContext &context,
                         llvm::IRBuilder<> &builder)
      : module(module), context(context), builder(builder) {}

  virtual std::string llvmStrategy() { return "shadow-stack"; }
  llvm::GlobalVariable *getFrameMapForCallee(FuncDefAST *);

  void setStackEntryFromCaller(FuncDefAST *, llvm::Function *);
  void relieveStackEntryByCallee(FuncDefAST *, llvm::Function *);

  void initGlobalRootChain();

  void initGCFunc();
};
} // namespace sammine_lang::AST
