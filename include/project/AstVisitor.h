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

using namespace llvm;

class AstVisitor {
public:
    virtual Value* VisitNumberExprAST(const NumberExprAST *AST) const = 0;
    virtual Value*  VisitCallExprAST(const CallExprAST *AST) const = 0;
    virtual Value*  VisitVariableExprAST(const VariableExprAST *AST) const = 0;
    virtual Value*  VisitBinaryExprAST(const BinaryExprAST *AST) const = 0;
    virtual Value*  VisitPrototypeAST(const PrototypeAST *AST) const = 0;
    virtual Value*  VisitFunctionAST(const FunctionAST *AST) const = 0;

};
#endif //SAMMINE_LANG_ASTVISITOR_H
