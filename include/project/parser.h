//
// Created by Jasmine Tang on 10/3/23.
//

#pragma once

#include "AST.h"
#include "lexer.h"
#include <map>
class parser {
public:
    static int CurTok;

    static int getNextToken();

    static inline std::map<char, int> BinopPrecedence =  {
            {'<', 10},
            {'+', 20},
            {'-', 20},
            {'*', 40}
    };


    static std::unique_ptr<ExprAST> LogError(const char *Str);
    static std::unique_ptr<ExprAST> LogErrorP(const char *Str);
    static std::unique_ptr<ExprAST> ParseExpression();
    static std::unique_ptr<ExprAST> ParseNumberExpr();

    static std::unique_ptr<ExprAST> ParseParenExpr();

    static std::unique_ptr<ExprAST> ParseIdentifierExpr();

    static std::unique_ptr<ExprAST> ParsePrimary();

    static int GetTokPrecedence();
};




