//
// Created by Jasmine Tang on 10/3/23.
//

#include "parser.h"



std::unique_ptr<ExprAST> parser::LogError(const char *Str) {
    fprintf(stderr, "Error: %s\n", Str);

    return nullptr;
}

std::unique_ptr<ExprAST> parser::LogErrorP(const char *Str) {
    LogError(Str);
    return nullptr;
}

int parser::getNextToken() {
    return CurTok = lexer::gettok();
}

std::unique_ptr<ExprAST> parser::ParseNumberExpr() {
    auto Result =  std::make_unique<NumberExprAST>(lexer::NumVal);

    parser::getNextToken();
    return std::move(Result);
}

std::unique_ptr<ExprAST> parser::ParseParenExpr() {
    parser::getNextToken();

    auto V = parser::ParseExpression();

    if (!V) return nullptr;

    if (parser::CurTok != ')') return parser::LogError("expected ')'");
    parser::getNextToken();

    return V;
}

std::unique_ptr<ExprAST> parser::ParseIdentifierExpr() {
    std::string IdName = lexer::IdentifierStr;

    getNextToken();

    if (CurTok != '(')
        return std::make_unique<VariableExprAST>(IdName);

    getNextToken();

    std::vector<std::unique_ptr<ExprAST>> Args;

    if (parser::CurTok != ')') {
        while (true) {
            if (auto Arg = ParseExpression()) {
                Args.push_back(std::move(Arg));
            } else {
                return nullptr;
            }

            if (CurTok == ')')
                break;

            if (CurTok != ',') {
                return LogError("Expected ')' or ',' in argument list");
            }

            parser::getNextToken();
        }
    }
    parser::getNextToken();

    return std::make_unique<CallExprAST>(IdName, std::move(Args));
}

std::unique_ptr<ExprAST> parser::ParsePrimary() {
    switch (parser::CurTok) {
        default:
            return LogError("Unknown token when expecting an expression");
        case tok_identifier:
            return ParseIdentifierExpr();
        case tok_number:
            return ParseNumberExpr();
        case '(':
            return ParseParenExpr();
    }
}
int parser::GetTokPrecedence() {
    if (!isascii(CurTok))
        return -1;

    // Make sure it's a declared binop.
    int TokPrec = BinopPrecedence[CurTok];
    if (TokPrec <= 0) return -1;
    return TokPrec;
}

std::unique_ptr<ExprAST> parser::ParseExpression() {
    auto LHS = ParsePrimary();

    if (!LHS) return nullptr;

    return ParseBinOpRHS(0, std::move(LHS));
}




