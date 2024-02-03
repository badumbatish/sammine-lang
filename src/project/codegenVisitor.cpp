//
// Created by jjsm on 2/2/24.
//

#include "codegenVisitor.h"
#include <iostream>
Value *CodeGenVisitor::Visit(const NumberExprAST *AST) const {
    return ConstantFP::get(*TheContext, APFloat(AST->getVal()));
}

Value *CodeGenVisitor::Visit(const CallExprAST *AST) const {
        // Look up the name in the global module table.
        Function *CalleeF = TheModule->getFunction(AST->Callee);
        if (!CalleeF) {
            std::cout << "Unknown function referenced" << std::endl;
            return nullptr;
        }

        // If argument mismatch error.
        if (CalleeF->arg_size() != AST->Args.size()){
            std::cout << "Function argument mismatched in size" << std::endl;
            return nullptr;
        }
        std::vector<Value *> ArgsV;
        for (unsigned i = 0, e = AST->Args.size(); i != e; ++i) {
            ArgsV.push_back(AST->Args[i]->Accept((AstVisitor*)      this));
            if (!ArgsV.back())
                return nullptr;
        }

        return Builder->CreateCall(CalleeF, ArgsV, "calltmp");
}

Value *CodeGenVisitor::Visit(const VariableExprAST *AST) {
    return NamedValues[AST->getName()];
}

Value *CodeGenVisitor::Visit(const BinaryExprAST *AST) {
    Value *L = AST->LHS->Accept((AstVisitor*) (this));
    Value *R = AST->RHS->Accept((AstVisitor*) (this));
    if (!L || !R)
        return nullptr;

    auto op = AST->Op;
    if (op == "+")
            return Builder->CreateFAdd(L, R, "addtmp");
    else if (op == "-")
            return Builder->CreateFSub(L, R, "subtmp");
    else if (op == "*")
            return Builder->CreateFMul(L, R, "multmp");
    else if (op == "<") {
        L = Builder->CreateFCmpULT(L, R, "cmptmp");
        // Convert bool 0/1 to double 0.0 or 1.0
        return Builder->CreateUIToFP(L, Type::getDoubleTy(*TheContext),
                                         "booltmp");
    }
    else return nullptr;

}

Function *CodeGenVisitor::Visit(const PrototypeAST *AST) const {
    // Make the function type:  double(double,double) etc.
    std::vector<Type *> Doubles(AST->Args.size(), Type::getDoubleTy(*TheContext));
    FunctionType *FT =
            FunctionType::get(Type::getDoubleTy(*TheContext), Doubles, false);

    Function *F =
            Function::Create(FT, Function::ExternalLinkage, AST->Name, TheModule.get());

    // Set names for all arguments.
    unsigned Idx = 0;
    for (auto &Arg : F->args())
        Arg.setName(AST->Args[Idx++]);

    return F;
}

Function *CodeGenVisitor::Visit(const FunctionAST *AST) {
    Function *TheFunction = TheModule->getFunction(AST->Proto->getName());

    if (!TheFunction)
        TheFunction = AST->Proto->Accept((AstVisitor*) this);

    if (!TheFunction)
        return nullptr;

    // Create a new basic block to start insertion into.
    BasicBlock *BB = BasicBlock::Create(*TheContext, "entry", TheFunction);
    Builder->SetInsertPoint(BB);

    // Record the function arguments in the NamedValues map.
    NamedValues.clear();
    for (auto &Arg : TheFunction->args())
        NamedValues[std::string(Arg.getName())] = &Arg;

    if (Value *RetVal = AST->Body->Accept((AstVisitor*) this)) {
        // Finish off the function.
        Builder->CreateRet(RetVal);

        // Validate the generated code, checking for consistency.
        verifyFunction(*TheFunction);

        return TheFunction;
    }

    // Error reading body, remove function.
    TheFunction->eraseFromParent();
    return nullptr;
}

Value *CodeGenVisitor::Visit(const ExprAST *AST) const {
    return nullptr;
}

