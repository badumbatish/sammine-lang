#pragma once

#include <AST.h>
#include <Lexer.h>
#include "codegenVisitor.h"
#include "LLVMResource.h"
#include <map>
#include <utility>

static std::map<std::string, int> binopPrecedence = { {"<", 10},
                                        {"+", 20},
                                        {"-", 20},
                                        {"*", 40}};

class Parser {
private:
    Lexer lxr;
    std::shared_ptr<LLVMResource> llvmResource;
    std::shared_ptr<CodeGenVisitor> cgVisitor;
public:
    explicit Parser (Lexer _lxr) : lxr(std::move(_lxr)),
                                        llvmResource(std::make_shared<LLVMResource>()),
                                        cgVisitor(std::make_shared<CodeGenVisitor>(llvmResource)) {

    }
    int GetTokPrecedence(Token tok);
    std::unique_ptr<ExprAST> ParseNumberExpr();
    std::unique_ptr<ExprAST> ParseParenExpr();
    std::unique_ptr<ExprAST> ParseIdentifierExpr();
    std::unique_ptr<ExprAST> ParsePrimary();
    std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS);
    std::unique_ptr<ExprAST> ParseExpression();
    std::unique_ptr<PrototypeAST> ParsePrototype();
    std::unique_ptr<FunctionAST> ParseDefinition();
    std::unique_ptr<FunctionAST> ParseTopLevelExpr();
    std::unique_ptr<PrototypeAST> ParseExtern();

    void HandleDefinition();
    void HandleExtern();
    void HandleTopLevelExpression();
    void parse();


};

