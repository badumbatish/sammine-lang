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

using namespace llvm;
class ExprAST {
public:
    virtual ~ExprAST() {}
    virtual Value * Accept(std::shared_ptr<AstVisitor> visitor) const = 0;
};

class NumberExprAST : public ExprAST {
    double Val;

public:
    explicit NumberExprAST(double Val) : Val(Val) {}
    Value * Accept(std::shared_ptr<AstVisitor> visitor) const override;
};

class VariableExprAST : public ExprAST {
    std::string Name;

public:
    explicit VariableExprAST(const std::string &Name) : Name(Name) {}
    Value * Accept(std::shared_ptr<AstVisitor> visitor) const override;

};

class BinaryExprAST : public ExprAST {
    std::string Op;
    std::unique_ptr<ExprAST> LHS, RHS;

public:
    BinaryExprAST(std::string op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS) : Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
    Value * Accept(std::shared_ptr<AstVisitor> visitor) const override;

};


class CallExprAST : public ExprAST {
    std::string Callee;
    std::vector<std::unique_ptr<ExprAST>> Args;

public:
    CallExprAST(const std::string &Callee, std::vector<std::unique_ptr<ExprAST>> Args) : Callee(Callee), Args(std::move(Args)) {}
    Value * Accept(std::shared_ptr<AstVisitor> visitor) const override;
};

class PrototypeAST : public ExprAST {
    std::string Name;
    std::vector<std::string> Args;

public:
    PrototypeAST(const std::string &Name, std::vector<std::string> Args)
            : Name(Name), Args(std::move(Args)) {}

    [[nodiscard]] const std::string &getName() const { return Name; }
    Value * Accept(std::shared_ptr<AstVisitor> visitor) const override;

};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST : public ExprAST {
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;

public:
    FunctionAST(std::unique_ptr<PrototypeAST> Proto,
                std::unique_ptr<ExprAST> Body)
            : Proto(std::move(Proto)), Body(std::move(Body)) {}
    Value * Accept(std::shared_ptr<AstVisitor> visitor) const override;
};