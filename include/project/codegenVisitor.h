//
// Created by jjsm on 2/2/24.
//

#ifndef SAMMINE_LANG_CODEGENVISITOR_H
#define SAMMINE_LANG_CODEGENVISITOR_H
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
#include "grammar.h"
#include "AstVisitor.h"
#include <memory>
#include <map>
using namespace llvm;
class CodeGenVisitor {
public:
    std::unique_ptr<LLVMContext> TheContext;
    std::unique_ptr<IRBuilder<>> Builder;
    std::unique_ptr<Module> TheModule;
    std::map<std::string, Value *> NamedValues;

    CodeGenVisitor() {
        TheContext = std::make_unique<LLVMContext>();
        TheModule = std::make_unique<Module>("i jit on my code hehe", *TheContext);

        Builder = std::make_unique<IRBuilder<>>(*TheContext);

        NamedValues = {};
    }

    Value* VisitNumberExprAST(NumberExprAST *AST) const {};
    Value* VisitCallExprAST(CallExprAST *AST) const {};
    Value* VisitVariableExprAST(VariableExprAST *AST) const {};
    Value* VisitBinaryExprAST(BinaryExprAST *AST) const {};
    Value* VisitPrototypeAST(PrototypeAST *AST) const {};
    Value* VisitFunctionAST(FunctionAST *AST) const {};

};
#endif //SAMMINE_LANG_CODEGENVISITOR_H
