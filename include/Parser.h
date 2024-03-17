#include "Lexer.h"
#include "Ast.h"
namespace sammine_lang {
    class Parser {
    private:
        std::shared_ptr<TokenStream> tokStream;

        auto ParseProgram() -> std::shared_ptr<AST::ProgramAST>;

        // Parse definition
        auto ParseDefinition() -> std::shared_ptr<AST::DefinitionAST>;
        auto ParseFuncDef() -> std::shared_ptr<AST::FuncDefAST>;
        auto ParseVarDef() -> std::shared_ptr<AST::FuncDefAST>;

        // Parse type
        auto ParseTypedVar() -> std::shared_ptr<AST::TypedVarAST>;

        // Parse expressions
        auto ParseExpr() -> std::shared_ptr<AST::ExprAST>;
        auto ParseBinaryExpr() -> std::shared_ptr<AST::BinaryExprAST>;
        auto ParseCallExpr() -> std::shared_ptr<AST::CallExprAST>;
        auto ParseNumberExpr() ->std::shared_ptr<AST::NumberExprAST>;
        auto ParseVariableExpr() -> std::shared_ptr<AST::VariableExprAST>;

        // Parse block
        auto ParseBlock() -> std::shared_ptr<AST::BlockAST>;

        // Parse Statements
        auto ParseStmt() -> std::shared_ptr<AST::StmtAST>;
        auto ParseSimpleStmt() -> std::shared_ptr<AST::SimpleStmtAST>;
        auto ParseIfStmtStmt() -> std::shared_ptr<AST::IfStmtAST>;


    public:
        Parser(std::shared_ptr<TokenStream> tokStream) : tokStream(tokStream) {}
        auto Parse() -> std::shared_ptr<AST::ProgramAST>;
    };
}
