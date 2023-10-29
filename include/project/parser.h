//
// Created by Jasmine Tang on 10/3/23.
//

#pragma once

#include "AST.h"
#include "lexer.h"
#include <map>
class parser {
public:
    static inline int CurTok {};

    static int getNextToken();

    static inline std::map<char, int> BinopPrecedence = {
            {'<', 10},
            {'+', 20},
            {'-', 20},
            {'*', 40}
    };


    static std::unique_ptr<ExprAST> LogError(const char *Str);

    static std::unique_ptr<PrototypeAST> LogErrorP(const char *Str);

    static std::unique_ptr<ExprAST> ParseExpression();

    static std::unique_ptr<ExprAST> ParseNumberExpr();

    static std::unique_ptr<ExprAST> ParseParenExpr();

    static std::unique_ptr<ExprAST> ParseIdentifierExpr();

    static std::unique_ptr<ExprAST> ParsePrimary();

    static std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS);

    static std::unique_ptr<FunctionAST> ParseDefinition();

    static std::unique_ptr<PrototypeAST> ParseExtern();

    static std::unique_ptr<FunctionAST> ParseTopLevelExpr();
    static std::unique_ptr<PrototypeAST> ParsePrototype();

    static void MainLoop();
    static void InitializeModule();
    static void HandleDefinition();

    static int GetTokPrecedence();

    static void HandleExtern();

    static void HandleTopLevelExpression();

    static llvm::Value* LogErrorV(const char *Str);
};