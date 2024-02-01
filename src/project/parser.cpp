#include "parser.h"
#include "iostream"
std::unique_ptr<PrototypeAST> Parser::ParseExtern() {
    lxr.consume_current_token();
    return ParsePrototype();
}
int Parser::GetTokPrecedence(Token tok) {
    int TokPrec = binopPrecedence[tok.second];
    if (TokPrec <= 0) return -1;
    return TokPrec;
}

std::unique_ptr<PrototypeAST> Parser::ParsePrototype() {
    // we expect to get a token of identifier type when we call in
    auto [iden_token, iden_val] = lxr.peek_current_token();
    if (iden_token != TokenType::tok_identifier) {
        std::cout << "Expected function name in prototype" << std::endl;
        return nullptr;
    }
    lxr.consume_current_token();

    auto [opar_token, opar_val] = lxr.peek_current_token();
    if (opar_token != TokenType::tok_opar) {
        std::cout << "Expected open parenthesis in prototype" << std::endl;
        return nullptr;
    }
    lxr.consume_current_token();

    std::vector<std::string> ArgNames;

    while (lxr.peek_current_token().first == TokenType::tok_identifier) {
        ArgNames.push_back(lxr.peek_current_token().second);
        lxr.consume_current_token();
    }

    auto [epar_token, epar_val] = lxr.peek_current_token();
    if (epar_token != TokenType::tok_epar) {
        std::cout << "Expected close parenthesis in prototype" << std::endl;
        return nullptr;
    }
    lxr.consume_current_token();

    return std::make_unique<PrototypeAST>(iden_val, ArgNames);
}

std::unique_ptr<ExprAST> Parser::ParseNumberExpr() {
    auto [num_token, num_val] = lxr.peek_current_token();
    auto result =  std::make_unique<NumberExprAST>(std::stod(num_val));
    lxr.consume_current_token();
    return result;
}

std::unique_ptr<FunctionAST> Parser::ParseDefinition() {
    lxr.consume_current_token();
    auto Proto = ParsePrototype();

    if (!Proto) return nullptr;

    if (auto E = ParseExpression())
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));

    return nullptr;
}

std::unique_ptr<ExprAST> Parser::ParseExpression() {
    auto LHS = ParsePrimary();

    if (!LHS) return nullptr;

    return ParseBinOpRHS(0, std::move(LHS));
}

std::unique_ptr<ExprAST> Parser::ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS) {
    while (true) {
        int TokPrec = GetTokPrecedence(lxr.peek_current_token());

        if (TokPrec < ExprPrec) return LHS;

        Token binOpToken = lxr.peek_current_token();
        lxr.consume_current_token();

        auto RHS = ParsePrimary();
        if (!RHS) return nullptr;

        int NextPrec = GetTokPrecedence(lxr.peek_current_token());
        if (TokPrec < NextPrec) {
            RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
            if (!RHS) return nullptr;
        }

        LHS = std::make_unique<BinaryExprAST>(binOpToken.second, std::move(LHS), std::move(RHS));
    }
}

std::unique_ptr<ExprAST> Parser::ParsePrimary() {
    auto tok = lxr.peek_current_token();

    switch (tok.first) {
        default:
            std::cout << "Unknown token when parsing primary" << std::endl;
            return nullptr;
        case TokenType::tok_identifier:
            return ParseIdentifierExpr();
        case TokenType::tok_number:
            return ParseNumberExpr();
        case TokenType::tok_opar:
            return ParseParenExpr();
    }
}

std::unique_ptr<ExprAST> Parser::ParseParenExpr() {
    auto [opar_token, opar_val] = lxr.peek_current_token();
    if (opar_token != TokenType::tok_opar) {
        std::cout << "Expected open parenthesis in ParseParen" << std::endl;
        return nullptr;
    }
    lxr.consume_current_token();

    auto V = ParseExpression();
    if (!V) return nullptr;

    auto [epar_token, epar_val] = lxr.peek_current_token();
    if (epar_token != TokenType::tok_epar) {
        std::cout << "Expected close parenthesis in ParseParen" << std::endl;
        return nullptr;
    }
    lxr.consume_current_token();

    return V;
}

std::unique_ptr<ExprAST> Parser::ParseIdentifierExpr() {
    auto [iden_token, iden_val] = lxr.peek_current_token();
    if (iden_token != TokenType::tok_identifier) {
        std::cout << "Expected identifier in ParseIdentifierExpr" << std::endl;
        return nullptr;
    }
    lxr.consume_current_token();

    auto [opar_token, opar_val] = lxr.peek_current_token();
    if (opar_token != TokenType::tok_opar) {
        return std::make_unique<VariableExprAST>(iden_val);
    }
    lxr.consume_current_token();

    auto [tok, val] = lxr.peek_current_token();
    std::vector<std::unique_ptr<ExprAST>> Args;

    if (tok != TokenType::tok_epar) {
        while (true) {
            if (auto Arg = ParseExpression())
                Args.push_back(std::move(Arg));
            else return nullptr;

            auto [tok, val] = lxr.peek_current_token();

            if (tok == TokenType::tok_epar) break;

            if (tok != TokenType::tok_comma) {
                std::cout << "Expected close parenthesis or comma in argument list" << std::endl;
                return nullptr;
            }

            lxr.consume_current_token();

        }
    }

    lxr.consume_current_token();

    return std::make_unique<CallExprAST>(iden_val, std::move(Args));
}
std::unique_ptr<FunctionAST> Parser::ParseTopLevelExpr() {
    if (auto E = ParseExpression()) {
        auto Proto =  std::make_unique<PrototypeAST>("", std::vector<std::string>());
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    }
    return nullptr;
}

void Parser::HandleDefinition() {
    if (ParseDefinition()) {
        fprintf(stderr, "Parsed a function definition.\n");
    } else {
        // Skip token for error recovery.
        lxr.consume_current_token();
    }
}

void Parser::HandleExtern() {
    if (ParseExtern()) {
        fprintf(stderr, "Parsed an extern\n");
    } else {
        // Skip token for error recovery.
        lxr.consume_current_token();
    }
}

void Parser::HandleTopLevelExpression() {
    // Evaluate a top-level expression into an anonymous function.
    if (ParseTopLevelExpr()) {
        fprintf(stderr, "Parsed a top-level expr\n");
    } else {
        // Skip token for error recovery.
        lxr.consume_current_token();
    }
}
void Parser::parse() {
    while (true) {
        Token tok = this->lxr.peek_current_token();
        switch (tok.first) {
            case tok_def:
                HandleDefinition();
                break;
            case tok_extern:
                HandleExtern();
                break;
            case tok_semicolon:
                this->lxr.consume_current_token();
                break;
            case tok_eof:
                return;
            default:
                HandleTopLevelExpression();
                break;
        }
    }
}


