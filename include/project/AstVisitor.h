//
// Created by jjsm on 2/2/24.
//

#ifndef SAMMINE_LANG_ASTVISITOR_H
#define SAMMINE_LANG_ASTVISITOR_H

#include "AST.h"
#include "grammar.h"

class ExprAST;
class NumberExprAST;
class CallExprAST;
class VariableExprAST;
class BinaryExprAST;
class PrototypeAST;
class FunctionAST;

#include "AstVisitor.h"
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
#include <map>
using namespace llvm;

class AstVisitor {


public:
    std::unique_ptr<LLVMContext> TheContext;
    std::unique_ptr<IRBuilder<>> Builder;
    std::unique_ptr<Module> TheModule;
    std::map<std::string, Value *> NamedValues;
    AstVisitor() = default;
    virtual ~AstVisitor() = default;
    virtual Value* Visit(const ExprAST *AST) const = 0;
    virtual Value* Visit(const NumberExprAST *AST) const = 0;
    virtual Value*  Visit(const CallExprAST *AST) const = 0;
    virtual Value*  Visit(const VariableExprAST *AST) = 0;
    virtual Value*  Visit(const BinaryExprAST *AST) = 0;
    virtual Function*  Visit(const PrototypeAST *AST) const = 0;
    virtual Function*  Visit(const FunctionAST *AST) = 0;

};



#endif //SAMMINE_LANG_ASTVISITOR_H
