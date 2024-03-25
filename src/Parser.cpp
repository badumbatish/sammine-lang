
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
        if (prototype == nullptr) return nullptr;

        auto block = ParseBlock();
        if (block == nullptr) return nullptr;
        return std::make_unique<AST::FuncDefAST>(std::move(prototype), std::move(block));
    }

    //! Parsing implementation for a variable decl/def

    //! Accepts a let, continue parsing inside and (enable error reporting if possible).
    //! If a `let` is not found then return a nullptr.
    auto Parser::ParseVarDef() -> std::unique_ptr<AST::DefinitionAST> {
        auto let = expect(TokenType::TokLet);
        // TODO: Add stopping nullpr checker
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
        if (fn == nullptr) return nullptr;
        // TODO: Please add error reporting after this point
        auto id = expect(TokID);

        auto params = ParseParams();
        auto arrow = expect(TokArrow);
        auto returnType = expect(TokID);

        return std::make_unique<AST::PrototypeAST>(id->lexeme, returnType->lexeme, std::move(params));
    }

    auto Parser::ParseBlock() -> std::unique_ptr<AST::BlockAST> {

        auto blockAST = std::make_unique<AST::BlockAST>();
        auto leftCurly = expect(TokLeftCurly);
        // TODO : Cannot just parse a return stmt.
        // TODO : We need to also parse other statement as well
        auto returnStmt = ParseReturnStmt();
        auto rightCurly = expect(TokRightCurly);

        blockAST->returnStmt = std::move(returnStmt);
        return blockAST;
    }

    auto Parser::ParseReturnStmt() -> std::unique_ptr<AST::ExprAST> {
        auto returnTok = expect(TokReturn);
        auto expr = ParseExpr();
        auto semiColon = expect(TokSemiColon);
        return expr;
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

    // Parsing of parameters in a function call, we use leftParen and rightParen as appropriate stopping point
    auto Parser::ParseParams() -> std::unique_ptr<std::vector<std::unique_ptr<AST::TypedVarAST>>> {
        auto leftParen = expect(TokLeftParen);
        if (leftParen == nullptr) return {};

        std::unique_ptr<std::vector<std::unique_ptr<AST::TypedVarAST>>> vec = std::make_unique<std::vector<std::unique_ptr<AST::TypedVarAST>>>();
        auto typeVar = ParseTypedVar();
        if (typeVar != nullptr) { vec->push_back(std::move(typeVar)); }
        while (true) {
            //auto typeVar = ParseTypedVar();
            auto comma = expect(TokComma);
            if (comma == nullptr) break;

            typeVar = ParseTypedVar();
            if (typeVar == nullptr) return {};
        }
        auto rightParen = expect(TokRightParen);
        if (rightParen == nullptr) return nullptr;

        return vec;
    }
    auto Parser::expect(TokenType tokType) -> std::shared_ptr<Token> {
        auto currentToken = tokStream->peek();
        if (!tokStream->isEnd() && currentToken->type == tokType) {
            return tokStream->consume();
        } else {
            // TODO: Add error reporting after this point.
            return nullptr;
        }
    }
}

