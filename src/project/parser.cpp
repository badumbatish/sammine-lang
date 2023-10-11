//
// Created by Jasmine Tang on 10/3/23.
//

#include "parser.h"


std::unique_ptr<ExprAST> parser::LogError(const char *Str) {
    fprintf(stderr, "Error: %s\n", Str);

    return nullptr;
}

std::unique_ptr<PrototypeAST> parser::LogErrorP(const char *Str) {
    LogError(Str);
    return nullptr;
}

int parser::getNextToken() {
    return CurTok = lexer::gettok();
}

std::unique_ptr<ExprAST> parser::ParseNumberExpr() {
    auto Result = std::make_unique<NumberExprAST>(lexer::NumVal);

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

std::unique_ptr<ExprAST> parser::ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS) {
    while (true) {
        int TokPrec = GetTokPrecedence();

        // If this is a binop that binds at least as tightly as the current binop,
        // consume it, otherwise we are done.
        if (TokPrec < ExprPrec)
            return LHS;
        // Okay, we know this is a binop.
        int BinOp = CurTok;
        getNextToken();  // eat binop

        // Parse the primary expression after the binary operator.
        auto RHS = ParsePrimary();
        if (!RHS)
            return nullptr;

        int NextPrec = GetTokPrecedence();
        if (TokPrec < NextPrec) {
        }
    }
}

std::unique_ptr<FunctionAST> parser::ParseDefinition() {
    getNextToken();  // eat def.
    auto Proto = ParsePrototype();
    if (!Proto) return nullptr;

    if (auto E = ParseExpression())
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    return nullptr;
}



/// external ::= 'extern' prototype
std::unique_ptr<PrototypeAST> parser::ParseExtern() {
    getNextToken();  // eat extern.
    return ParsePrototype();
}

std::unique_ptr<FunctionAST> parser::ParseTopLevelExpr() {
    if (auto E = ParseExpression()) {
        // Make an anonymous proto.
        auto Proto = std::make_unique<PrototypeAST>("", std::vector<std::string>());
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    }
    return nullptr;
}
void parser::HandleDefinition() {
    if (ParseDefinition()) {
        fprintf(stderr, "Parsed a function definition.\n");
    } else {
        // Skip token for error recovery.
        getNextToken();
    }
}
 void parser::HandleExtern() {
    if (ParseExtern()) {
        fprintf(stderr, "Parsed an extern\n");
    } else {
        // Skip token for error recovery.
        getNextToken();
    }
}

void parser::HandleTopLevelExpression() {
    // Evaluate a top-level expression into an anonymous function.
    if (ParseTopLevelExpr()) {
        fprintf(stderr, "Parsed a top-level expr\n");
    } else {
        // Skip token for error recovery.
        getNextToken();
    }
}
std::unique_ptr<PrototypeAST> parser::ParsePrototype() {
    if (CurTok != tok_identifier)
        return LogErrorP("Expected function name in prototype");

    std::string FnName = lexer::IdentifierStr;
    getNextToken();

    if (parser::CurTok != '(')
        return LogErrorP("Expected '(' in prototype");

    // Read the list of argument names.
    std::vector<std::string> ArgNames;
    while (getNextToken() == tok_identifier)
        ArgNames.push_back(lexer::IdentifierStr);
    if (CurTok != ')')
        return LogErrorP("Expected ')' in prototype");

    // success.
    getNextToken();  // eat ')'.

    return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
}

void parser::MainLoop() {
    while (true) {
        fprintf(stderr, "ready> ");
        switch (CurTok) {
            case tok_eof:
                return;
            case ';': // ignore top-level semicolons.
                getNextToken();
                break;
            case tok_def:
                HandleDefinition();
                break;
            case tok_extern:
                HandleExtern();
                break;
            default:
                HandleTopLevelExpression();
                break;
        }
    }
}