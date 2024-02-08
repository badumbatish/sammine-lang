//
// Created by jjsm on 2/2/24.
//

#include "codegenVisitor.h"
#include "LLVMResource.h"

#include <iostream>
Value *CodeGenVisitor::Visit(const NumberExprAST *AST) const {
    return ConstantFP::get(*llvmResoucePtr->TheContext, APFloat(AST->getVal()));
}

Value * CodeGenVisitor::Visit(const CallExprAST *AST) {
        // Look up the name in the global module table.
        Function *CalleeF = getFunction(AST->Callee);
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

        return llvmResoucePtr->Builder->CreateCall(CalleeF, ArgsV, "calltmp");
}

Value *CodeGenVisitor::Visit(const VariableExprAST *AST) {
    return llvmResoucePtr->NamedValues[AST->getName()];
}

Value *CodeGenVisitor::Visit(const BinaryExprAST *AST) {
    Value *L = AST->LHS->Accept((AstVisitor*) (this));
    Value *R = AST->RHS->Accept((AstVisitor*) (this));
    if (!L || !R)
        return nullptr;

    auto op = AST->Op;
    if (op == "+")
            return llvmResoucePtr->Builder->CreateFAdd(L, R, "addtmp");
    else if (op == "-")
            return llvmResoucePtr->Builder->CreateFSub(L, R, "subtmp");
    else if (op == "*")
            return llvmResoucePtr->Builder->CreateFMul(L, R, "multmp");
    else if (op == "<") {
        L = llvmResoucePtr->Builder->CreateFCmpULT(L, R, "cmptmp");
        // Convert bool 0/1 to double 0.0 or 1.0
        return llvmResoucePtr->Builder->CreateUIToFP(L, Type::getDoubleTy(*(llvmResoucePtr->TheContext)),
                                         "booltmp");
    }
    else return nullptr;

}

Function *CodeGenVisitor::Visit(const PrototypeAST *AST) const {
    // Make the function type:  double(double,double) etc.
    std::vector<Type *> Doubles(AST->Args.size(), Type::getDoubleTy(*(llvmResoucePtr->TheContext)));
    FunctionType *FT =
            FunctionType::get(Type::getDoubleTy(*(llvmResoucePtr->TheContext)), Doubles, false);

    Function *F =
            Function::Create(FT, Function::ExternalLinkage, AST->Name, llvmResoucePtr->TheModule.get());

    // Set names for all arguments.
    unsigned Idx = 0;
    for (auto &Arg : F->args())
        Arg.setName(AST->Args[Idx++]);

    return F;
}

Function *CodeGenVisitor::Visit(const FunctionAST *AST) {
    Function *TheFunction = getFunction(AST->Proto->getName());

    if (!TheFunction)
        TheFunction = AST->Proto->Accept((AstVisitor*) this);

    if (!TheFunction)
        return nullptr;

    // Create a new basic block to start insertion into.
    BasicBlock *BB = BasicBlock::Create(*(llvmResoucePtr->TheContext), "entry", TheFunction);
    llvmResoucePtr->Builder->SetInsertPoint(BB);

    // Record the function arguments in the NamedValues map.
    llvmResoucePtr->NamedValues.clear();
    for (auto &Arg : TheFunction->args())
        llvmResoucePtr->NamedValues[std::string(Arg.getName())] = &Arg;

    if (Value *RetVal = AST->Body->Accept((AstVisitor*) this)) {
        // Finish off the function.
        llvmResoucePtr->Builder->CreateRet(RetVal);

        // Validate the generated code, checking for consistency.
        verifyFunction(*TheFunction);

        llvmResoucePtr->TheFPM->run(*TheFunction, *llvmResoucePtr->TheFAM);
        return TheFunction;
    }

    // Error reading body, remove function.
    TheFunction->eraseFromParent();
    return nullptr;
}

Value *CodeGenVisitor::Visit(const ExprAST *AST) const {
    return nullptr;
}

Function *CodeGenVisitor::getFunction(std::string Name) {
    // First, see if the function has already been added to the current module.
    if (auto *F = llvmResoucePtr->TheModule->getFunction(Name))
        return F;

    // If not, check whether we can codegen the declaration from some existing
    // prototype.
    auto FI = llvmResoucePtr->FunctionProtos.find(Name);
    if (FI != llvmResoucePtr->FunctionProtos.end())
        return FI->second->Accept((AstVisitor*) this);

    // If no existing prototype exists, return null.
    return nullptr;
}
