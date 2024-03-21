
#include <functional>
#include "Parser.h"

namespace sammine_lang {
    auto Parser::Parse() -> std::unique_ptr<AST::ProgramAST> {
        return ParseProgram();
    }

    auto Parser::ParseProgram() -> std::unique_ptr<AST::ProgramAST> {
        auto programAST = std::make_unique<AST::ProgramAST>();
        while (!tokStream->isEnd()) {
            auto def = ParseDefinition();
            if (def == nullptr) return nullptr;
            programAST->DefinitionVec.push_back(std::move(def));
        }

        return programAST;
    }

    auto Parser::ParseDefinition() -> std::unique_ptr<AST::DefinitionAST> {
        using ParseFunction = std::function<std::unique_ptr<AST::DefinitionAST> (Parser*)>;
        std::vector<std::pair<ParseFunction, bool>> ParseFunctions = {
                {&Parser::ParseFuncDef, false},
                {&Parser::ParseVarDef, false},
        };

        for (auto [fn, required] : ParseFunctions) {
            auto result = fn(this);
            if (result != nullptr) return result;
        }
        return nullptr;
    }

    auto Parser::ParseFuncDef() -> std::unique_ptr<AST::DefinitionAST> {
        auto prototype = ParsePrototype();
        auto block = ParseBlock();
        return {};
    }

    auto Parser::ParseVarDef() -> std::unique_ptr<AST::DefinitionAST> {
        auto let = expect(TokenType::TokLet);
        auto typedVar = ParseTypedVar();
        auto assign = expect(TokenType::TokASSIGN);
        auto expr = ParseExpr();
        auto semicolon = expect(TokenType::TokSemiColon);

        auto varDef = std::make_unique<AST::VarDefAST>(std::move(typedVar), std::move(expr));

        return varDef;
    }

    auto Parser::ParseTypedVar() -> std::unique_ptr<AST::TypedVarAST> {
        auto name = expect(TokenType::TokID);
        auto colon = expect(TokenType::TokColon);
        auto type = expect(TokenType::TokID);

        auto typedVar = std::make_unique<AST::TypedVarAST>();

        typedVar->name = name->lexeme;
        typedVar->type = type->lexeme;
        return typedVar;
    }

    auto Parser::ParseExpr() -> std::unique_ptr<AST::ExprAST> {
        using ParseFunction = std::function<std::unique_ptr<AST::ExprAST> (Parser*)>;
        std::vector<std::pair<ParseFunction, bool>> ParseFunctions = {
                {&Parser::ParseBinaryExpr, false},
                {&Parser::ParseCallExpr, false},
                {&Parser::ParseNumberExpr, false},
                {&Parser::ParseVariableExpr, false},
        };

        for (auto [fn, required] : ParseFunctions) {
            auto result = fn(this);
            if (result != nullptr) return result;
        }
        return nullptr;
    }

    auto Parser::ParseBinaryExpr() -> std::unique_ptr<AST::ExprAST> {
        return {};
    }

    auto Parser::ParseCallExpr() -> std::unique_ptr<AST::ExprAST> {
        return {};
    }

    auto Parser::ParseNumberExpr() -> std::unique_ptr<AST::ExprAST> {
        auto numberExpr = std::make_unique<AST::NumberExprAST>();

        auto numberToken = expect(TokenType::TokNum);

        if (numberToken == nullptr) {

        } else {
            numberExpr->number = numberToken->lexeme;
        }

        return numberExpr;
    }

    auto Parser::ParseVariableExpr() -> std::unique_ptr<AST::ExprAST> {
        return {};
    }

    auto Parser::ParsePrototype() -> std::unique_ptr<AST::PrototypeAST> {
        auto fn = expect(TokFunc);
        auto id = expect(TokID);
        auto leftParen = expect(TokLeftParen);
        auto rightParen = expect(TokRightParen);
        auto arrow = expect(TokArrow);
        return {};
    }

    auto Parser::ParseBlock() -> std::unique_ptr<AST::BlockAST> {
        return {};
    }

    auto Parser::ParseStmt() -> std::unique_ptr<AST::StmtAST> {
        return {};
    }

    auto Parser::ParseSimpleStmt() -> std::unique_ptr<AST::SimpleStmtAST> {
        return {};
    }

    auto Parser::ParseIfStmtStmt() -> std::unique_ptr<AST::IfStmtAST> {
        return {};
    }


    auto Parser::expect(TokenType tokType) -> std::shared_ptr<Token> {
        if (!tokStream->isEnd() && tokStream->peek()->type == tokType) {
            return tokStream->consume();
        } else {
            return nullptr;
        }
    }
}

