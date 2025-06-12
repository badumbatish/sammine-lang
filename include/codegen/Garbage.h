
#pragma once
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include <string>
class ShadowGarbageCollector {

  llvm::Module &module;
  llvm::LLVMContext &context;
  llvm::IRBuilder<> &builder;
  std::vector<llvm::Constant *> MetaDataEntries;

public:
  ShadowGarbageCollector(llvm::Module &module, llvm::LLVMContext &context,
                         llvm::IRBuilder<> &builder)
      : module(module), context(context), builder(builder) {}

  virtual std::string llvmStrategy() { return "shadow-stack"; }
  void createFrameMap(llvm::Function *);

  void createStackEntry(llvm::Function *);

  void initGlobalRootChain();

  void initGCFunc();
};
