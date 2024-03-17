
#include <functional>
#include "Parser.h"

namespace sammine_lang {
    auto Parser::Parse() -> std::shared_ptr<AST::ProgramAST> {
        return ParseProgram();
    }

    auto Parser::ParseProgram() -> std::shared_ptr<AST::ProgramAST> {
        auto programAST = std::make_shared<AST::ProgramAST>();
        while (!tokStream->isEnd()) {
            auto def = ParseDefinition();
            if (def == nullptr) return nullptr;
            programAST->DefinitionVec.push_back(def);
        }

        return programAST;
    }

    auto Parser::ParseDefinition() -> std::shared_ptr<AST::DefinitionAST> {
        using ParseFunction = std::function<std::shared_ptr<AST::DefinitionAST> (Parser*)>;
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

    auto Parser::ParseFuncDef() -> std::shared_ptr<AST::DefinitionAST> {
        return {};
    }

    auto Parser::ParseVarDef() -> std::shared_ptr<AST::DefinitionAST> {
        auto let = expect(TokenType::TokLet);
        auto typedVar = ParseTypedVar();
        auto assign = expect(TokenType::TokASSIGN);
        auto expr = ParseExpr();
        auto semicolon = expect(TokenType::TokSemiColon);

        auto varDef = std::make_shared<AST::VarDefAST>(typedVar, expr);

        return varDef;
    }

    auto Parser::ParseTypedVar() -> std::shared_ptr<AST::TypedVarAST> {
        auto name = expect(TokenType::TokID);
        auto colon = expect(TokenType::TokColon);
        auto type = expect(TokenType::TokID);

        auto typedVar = std::make_shared<AST::TypedVarAST>();

        typedVar->name = name->lexeme;
        typedVar->type = type->lexeme;
        return typedVar;
    }

    auto Parser::ParseExpr() -> std::shared_ptr<AST::ExprAST> {
        using ParseFunction = std::function<std::shared_ptr<AST::ExprAST> (Parser*)>;
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

    auto Parser::ParseBinaryExpr() -> std::shared_ptr<AST::ExprAST> {
        return {};
    }

    auto Parser::ParseCallExpr() -> std::shared_ptr<AST::ExprAST> {
        return {};
    }

    auto Parser::ParseNumberExpr() -> std::shared_ptr<AST::ExprAST> {
        auto numberExpr = std::make_shared<AST::NumberExprAST>();

        auto numberToken = expect(TokenType::TokNum);

        if (numberToken == nullptr) {

        } else {
            numberExpr->number = numberToken->lexeme;
        }

        return numberExpr;
    }

    auto Parser::ParseVariableExpr() -> std::shared_ptr<AST::ExprAST> {
        return {};
    }

    auto Parser::ParseBlock() -> std::shared_ptr<AST::BlockAST> {
        return {};
    }

    auto Parser::ParseStmt() -> std::shared_ptr<AST::StmtAST> {
        return {};
    }

    auto Parser::ParseSimpleStmt() -> std::shared_ptr<AST::SimpleStmtAST> {
        return {};
    }

    auto Parser::ParseIfStmtStmt() -> std::shared_ptr<AST::IfStmtAST> {
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

