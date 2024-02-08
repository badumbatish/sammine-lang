//
// Created by jjsm on 2/7/24.
//

#ifndef SAMMINE_LANG_LLVMRESOURCE_H
#define SAMMINE_LANG_LLVMRESOURCE_H

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/Analysis/CGSCCPassManager.h"
#include "llvm/Passes/StandardInstrumentations.h"
#include "llvm/Passes/PassBuilder.h"
#include "KaleidoscopeJIT.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Scalar/Reassociate.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"

#include <map>
using namespace llvm;
using namespace llvm::orc;
class LLVMResource {
public:
    ExitOnError ExitOnErr;
    std::unique_ptr<LLVMContext> TheContext;
    std::unique_ptr<IRBuilder<>> Builder;
    std::unique_ptr<Module> TheModule;
    std::map<std::string, Value *> NamedValues;

    std::unique_ptr<FunctionPassManager> TheFPM;
    std::unique_ptr<LoopAnalysisManager> TheLAM;
    std::unique_ptr<FunctionAnalysisManager> TheFAM;
    std::unique_ptr<CGSCCAnalysisManager> TheCGAM;
    std::unique_ptr<ModuleAnalysisManager> TheMAM;
    std::unique_ptr<PassInstrumentationCallbacks> ThePIC;
    std::unique_ptr<StandardInstrumentations>  TheSI;
    std::unique_ptr<llvm::orc::KaleidoscopeJIT> TheJIT;
    std::map<std::string, std::unique_ptr<PrototypeAST>> FunctionProtos;

    PassBuilder PB;
    LLVMResource() {
        InitializeNativeTarget();
        InitializeNativeTargetAsmPrinter();
        InitializeNativeTargetAsmParser();

        TheJIT = ExitOnErr(KaleidoscopeJIT::Create());
        InitializeModuleAndManagers();
    }

    void InitializeModuleAndManagers() {
        // Open a new context and module.

        TheContext = std::make_unique<LLVMContext>();
        TheModule = std::make_unique<Module>("KaleidoscopeJIT", *TheContext);
        TheModule->setDataLayout(TheJIT->getDataLayout());

        // Create a new builder for the module.
        Builder = std::make_unique<IRBuilder<>>(*TheContext);

        // Create new pass and analysis managers.
        TheFPM = std::make_unique<FunctionPassManager>();
        TheLAM = std::make_unique<LoopAnalysisManager>();
        TheFAM = std::make_unique<FunctionAnalysisManager>();
        TheCGAM = std::make_unique<CGSCCAnalysisManager>();
        TheMAM = std::make_unique<ModuleAnalysisManager>();
        ThePIC = std::make_unique<PassInstrumentationCallbacks>();
        TheSI = std::make_unique<StandardInstrumentations>(*TheContext,
                /*DebugLogging*/ true);
        TheSI->registerCallbacks(*ThePIC, TheMAM.get());

        // Add transform passes.
        // Do simple "peephole" optimizations and bit-twiddling optzns.
        TheFPM->addPass(InstCombinePass());
        // Reassociate expressions.
        TheFPM->addPass(ReassociatePass());
        // Eliminate Common SubExpressions.
        TheFPM->addPass(GVNPass());
        // Simplify the control flow graph (deleting unreachable blocks, etc).
        TheFPM->addPass(SimplifyCFGPass());

        // Register analysis passes used in these transform passes.
        PB.registerModuleAnalyses(*TheMAM);
        PB.registerFunctionAnalyses(*TheFAM);
        PB.crossRegisterProxies(*TheLAM, *TheFAM, *TheCGAM, *TheMAM);
    }


};
#endif //SAMMINE_LANG_LLVMRESOURCE_H
