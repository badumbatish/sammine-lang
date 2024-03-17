
#include "Parser.h"

namespace sammine_lang {
    auto Parser::Parse() -> std::shared_ptr<AST::ProgramAST> {
        return ParseProgram();
    }

    auto Parser::ParseProgram() -> std::shared_ptr<AST::ProgramAST> {

    }

    auto Parser::ParseDefinition() -> std::shared_ptr<AST::DefinitionAST> {
        return std::shared_ptr<AST::DefinitionAST>();
    }

    auto Parser::ParseFuncDef() -> std::shared_ptr<AST::FuncDefAST> {
        return std::shared_ptr<AST::FuncDefAST>();
    }

    auto Parser::ParseVarDef() -> std::shared_ptr<AST::FuncDefAST> {
        return std::shared_ptr<AST::FuncDefAST>();
    }

    auto Parser::ParseTypedVar() -> std::shared_ptr<AST::TypedVarAST> {
        return std::shared_ptr<AST::TypedVarAST>();
    }

    auto Parser::ParseExpr() -> std::shared_ptr<AST::ExprAST> {
        return std::shared_ptr<AST::ExprAST>();
    }

    auto Parser::ParseBinaryExpr() -> std::shared_ptr<AST::BinaryExprAST> {
        return std::shared_ptr<AST::BinaryExprAST>();
    }

    auto Parser::ParseCallExpr() -> std::shared_ptr<AST::CallExprAST> {
        return std::shared_ptr<AST::CallExprAST>();
    }

    auto Parser::ParseNumberExpr() -> std::shared_ptr<AST::NumberExprAST> {
        return std::shared_ptr<AST::NumberExprAST>();
    }

    auto Parser::ParseVariableExpr() -> std::shared_ptr<AST::VariableExprAST> {
        return std::shared_ptr<AST::VariableExprAST>();
    }

    auto Parser::ParseBlock() -> std::shared_ptr<AST::BlockAST> {
        return std::shared_ptr<AST::BlockAST>();
    }

    auto Parser::ParseStmt() -> std::shared_ptr<AST::StmtAST> {
        return std::shared_ptr<AST::StmtAST>();
    }

    auto Parser::ParseSimpleStmt() -> std::shared_ptr<AST::SimpleStmtAST> {
        return std::shared_ptr<AST::SimpleStmtAST>();
    }

    auto Parser::ParseIfStmtStmt() -> std::shared_ptr<AST::IfStmtAST> {
        return std::shared_ptr<AST::IfStmtAST>();
    }
}

