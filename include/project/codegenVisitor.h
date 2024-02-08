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
#include <iostream>
#include <map>
#include "LLVMResource.h"
using namespace llvm;
class CodeGenVisitor : AstVisitor {
private:
    std::shared_ptr<LLVMResource> llvmResoucePtr;
public:
    CodeGenVisitor() = default;

    explicit CodeGenVisitor(std::shared_ptr<LLVMResource> resource) {
        llvmResoucePtr = resource;
    }

    Value* Visit(const ExprAST *AST) const override;
    Value* Visit(const NumberExprAST *AST) const override;
    Value * Visit(const CallExprAST *AST) override;
    Value* Visit(const VariableExprAST *AST) override;
    Value* Visit(const BinaryExprAST *AST) override;
    Function* Visit(const PrototypeAST *AST) const override;
    Function* Visit(const FunctionAST *AST) override;
    Function *getFunction(std::string Name);
};
#endif //SAMMINE_LANG_CODEGENVISITOR_H
