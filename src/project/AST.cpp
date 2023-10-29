//
// Created by Jasmine Tang on 10/8/23.
//

#include "AST.h"
#include "parser.h"


llvm::Value *NumberExprAST::codegen() {
    return llvm::ConstantFP::get(*TheContext, llvm::APFloat(Val));
}

llvm::Value *VariableExprAST::codegen() {
    llvm::Value *V = NamedValues[VariableExprAST::Name];
    if (!V)
        parser::LogErrorV("Unknown variable name");
    else
        return V;
}

llvm::Value *BinaryExprAST::codegen() {
    llvm::Value *L = this->LHS->codegen();
    llvm::Value *R = this->RHS->codegen();

    if (!L || !R) {
        return nullptr;
    }

    switch (this->Op) {
        case '+' :
            return Builder->CreateFAdd(L, R, "addtmp");
        case '-' :
            return Builder->CreateFSub(L, R, "subtmp");
        case '*' :
            return Builder->CreateFMul(L, R, "multmp");
        case '<' :
            L = Builder->CreateFCmpULT(L, R, "cmptmp");
            return Builder->CreateUIToFP(L, llvm::Type::getDoubleTy(*TheContext), "booltmp");

        default:
            return parser::LogErrorV("invalid binary operator");
    }
}
llvm::Value *CallExprAST::codegen() {
    // Get the function with the name that is in the string Callee into the pointer
    llvm::Function *CalleeF = TheModule->getFunction(this->Callee);

    // If the pointer is null, meaning the function is not found
    if (!CalleeF)
        return parser::LogErrorV("Unknown function referenced");

    // If the
    if (CalleeF->arg_size() != Args.size())
        return parser::LogErrorV("Incorrect # of argument passed");

    std::vector<llvm::Value*> ArgsV;

    for(unsigned i = 0, e =   Args.size(); i != e; i++) {
        ArgsV.push_back(Args[i]->codegen());

        // if whatever we just passed in is not good
        if (!ArgsV.back())
            return nullptr;
    }

    // CreateCall requires a pointer to the function, and its argument
    return Builder->CreateCall(CalleeF, ArgsV, "calltmp");
}
llvm::Function *PrototypeAST::codegen() {
    std::vector<llvm::Type*> Doubles(Args.size(), llvm::Type::getDoubleTy(*TheContext));




    // Constructing a FunctionType via a pointer
    llvm::FunctionType *FT = llvm::FunctionType::get(llvm::Type::getDoubleTy(*TheContext), Doubles, false);



    /// Creates a new function and attaches it to a module.
    ///
    /// Places the function in the program address space as specified
    /// by the module's data layout.
    llvm::Function *F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, Name, TheModule.get());

    unsigned  Idx = 0;

    for (auto &Arg: F->args()) {
        Arg.setName(Args[Idx++]);
    }

    return F;
}

llvm::Function *FunctionAST::codegen() {
    llvm::Function *TheFunction = TheModule->getFunction(Proto->getName());

    if (!TheFunction)
        TheFunction = Proto->codegen();

    if (!TheFunction)
        return nullptr;


    llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "entry", TheFunction);

    Builder->SetInsertPoint(BB);

    NamedValues.clear();

    for (auto &Arg : TheFunction->args())
        NamedValues[std::string(Arg.getName())] = &Arg;

    if (llvm::Value *RetVal = Body->codegen()) {
        Builder->CreateRet(RetVal);

        llvm::verifyFunction(*TheFunction);

        return TheFunction;
    }

    TheFunction->eraseFromParent();
    return nullptr;
}