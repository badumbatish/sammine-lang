#pragma once

#include "SammineJIT.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/Analysis/CGSCCPassManager.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/StandardInstrumentations.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Scalar/Reassociate.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"

namespace sammine_lang {
class LLVMRes {
public:
  llvm::ExitOnError ExitOnErr;
  std::unique_ptr<llvm::LLVMContext> Context;
  std::unique_ptr<llvm::IRBuilder<>> Builder;
  std::unique_ptr<llvm::Module> Module;
  std::map<std::string, llvm::Value *> NamedValues;

  std::unique_ptr<llvm::FunctionPassManager> FnPass;
  std::unique_ptr<llvm::LoopAnalysisManager> LpAnalysis;
  std::unique_ptr<llvm::FunctionAnalysisManager> FnAnalysis;
  std::unique_ptr<llvm::CGSCCAnalysisManager> CgAnalysis;
  std::unique_ptr<llvm::ModuleAnalysisManager> ModuleAnalysis;
  std::unique_ptr<llvm::PassInstrumentationCallbacks> PassCallbacks;
  std::unique_ptr<llvm::StandardInstrumentations> StdIns;
  std::unique_ptr<SammineJIT> sammineJIT;
  std::map<std::string, std::unique_ptr<sammine_lang::AST::PrototypeAST>>
      FnProto;

  llvm::PassBuilder PB;

  LLVMRes() {
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    sammineJIT = ExitOnErr(SammineJIT::Create());
    InitializeModuleAndManagers();
  }

  void InitializeModuleAndManagers() {
    InitializeEssentials();

    InitializeManagers();

    InitializeInstrs();

    // Add transform passes.
    // Do simple "peephole" optimizations and bit-twiddling optzns.
    //  FnPass->addPass(llvm::InstCombinePass());
    //  // Reassociate expressions.
    //  FnPass->addPass(llvm::ReassociatePass());
    //  // Eliminate Common SubExpressions.
    //  FnPass->addPass(llvm::GVNPass());
    //  // Simplify the control flow graph (deleting unreachable blocks, etc).
    //  FnPass->addPass(llvm::SimplifyCFGPass());

    InitializePassBuilder();
  }
  void InitializeEssentials() {
    // Open a new context and module.

    Context = std::make_unique<llvm::LLVMContext>();
    Module = std::make_unique<llvm::Module>("KaleidoscopeJIT", *Context);
    Module->setDataLayout(sammineJIT->getDataLayout());

    // Create a new builder for the module.
    Builder = std::make_unique<llvm::IRBuilder<>>(*Context);
  }
  void InitializeManagers() {
    // Create new pass and analysis managers.
    FnPass = std::make_unique<llvm::FunctionPassManager>();
    LpAnalysis = std::make_unique<llvm::LoopAnalysisManager>();
    FnAnalysis = std::make_unique<llvm::FunctionAnalysisManager>();
    CgAnalysis = std::make_unique<llvm::CGSCCAnalysisManager>();
    ModuleAnalysis = std::make_unique<llvm::ModuleAnalysisManager>();
  }

  void InitializeInstrs() {
    PassCallbacks = std::make_unique<llvm::PassInstrumentationCallbacks>();

    StdIns =
        std::make_unique<llvm::StandardInstrumentations>(*Context,
                                                         /*DebugLogging*/ true);
    StdIns->registerCallbacks(*PassCallbacks, ModuleAnalysis.get());
  }
  void InitializePassBuilder() {
    // Register analysis passes used in these transform passes.
    PB.registerModuleAnalyses(*ModuleAnalysis);
    PB.registerFunctionAnalyses(*FnAnalysis);
    PB.crossRegisterProxies(*LpAnalysis, *FnAnalysis, *CgAnalysis,
                            *ModuleAnalysis);
  }
};
} // namespace sammine_lang
