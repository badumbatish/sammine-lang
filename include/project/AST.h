#pragma once
#include <string>
#include <memory>
#include <vector>
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
class AstVisitor;

using namespace llvm;
class ExprAST {
public:
    virtual ~ExprAST() {}
    virtual Value * Accept(AstVisitor* v) const = 0;
};

class NumberExprAST : public ExprAST {
    double Val;

public:
    explicit NumberExprAST(double Val) : Val(Val) {}
    double getVal() const { return Val; }
    Value * Accept(AstVisitor* v) const override;
};

class VariableExprAST : public ExprAST {
    std::string Name;

public:
    explicit VariableExprAST(const std::string &Name) : Name(Name) {}
    std::string getName() const { return Name; }
    Value * Accept(AstVisitor* v) const override;

};

class BinaryExprAST : public ExprAST {
public:
    std::string Op;
    std::unique_ptr<ExprAST> LHS, RHS;
    BinaryExprAST(std::string op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS) : Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
    Value * Accept(AstVisitor* v) const override;
};


class CallExprAST : public ExprAST {
public:
    std::string Callee;
    std::vector<std::unique_ptr<ExprAST>> Args;
    CallExprAST(const std::string &Callee, std::vector<std::unique_ptr<ExprAST>> Args) : Callee(Callee), Args(std::move(Args)) {}
    Value * Accept(AstVisitor* v) const override;
};

class PrototypeAST {
public:
    std::string Name;
    std::vector<std::string> Args;
    PrototypeAST(const std::string &Name, std::vector<std::string> Args)
            : Name(Name), Args(std::move(Args)) {}

    [[nodiscard]] const std::string &getName() const { return Name; }
    Function * Accept(AstVisitor* v) const;

};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST  {
public:
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;
    FunctionAST(std::unique_ptr<PrototypeAST> Proto,
                std::unique_ptr<ExprAST> Body)
            : Proto(std::move(Proto)), Body(std::move(Body)) {}
    Function * Accept(AstVisitor* v) const ;
};