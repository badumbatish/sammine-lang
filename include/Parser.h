#include "Lexer.h"
#include "Ast.h"
namespace sammine_lang {
    class Parser {
    private:
        std::shared_ptr<TokenStream> tokStream;
        auto ParseProgram() -> std::unique_ptr<AST::ProgramAST>;

        // Parse definition
        auto ParseDefinition() -> std::unique_ptr<AST::DefinitionAST>;
        auto ParsePrototype() -> std::unique_ptr<AST::PrototypeAST>;
        auto ParseFuncDef() -> std::unique_ptr<AST::DefinitionAST>;
        auto ParseVarDef() -> std::unique_ptr<AST::DefinitionAST>;

        // Parse type
        auto ParseTypedVar() -> std::unique_ptr<AST::TypedVarAST>;

        // Parse expressions
        auto ParseExpr() -> std::unique_ptr<AST::ExprAST>;
        auto ParseBinaryExpr() -> std::unique_ptr<AST::ExprAST>;
        auto ParseCallExpr() -> std::unique_ptr<AST::ExprAST>;
        auto ParseNumberExpr() ->std::unique_ptr<AST::ExprAST>;
        auto ParseVariableExpr() -> std::unique_ptr<AST::ExprAST>;

        // Parse block
        auto ParseBlock() -> std::unique_ptr<AST::BlockAST>;

        // Parse Statements
        auto ParseStmt() -> std::unique_ptr<AST::StmtAST>;
        auto ParseSimpleStmt() -> std::unique_ptr<AST::SimpleStmtAST>;
        auto ParseIfStmtStmt() -> std::unique_ptr<AST::IfStmtAST>;
        auto ParseReturnStmt() -> std::unique_ptr<AST::ExprAST>;

        // Parse parameters
        auto ParseParams() -> std::unique_ptr<std::vector<std::unique_ptr<AST::TypedVarAST>>>;


        // Utilities
        auto expect(TokenType tokType) -> std::shared_ptr<Token>;
    public:
        std::vector<std::string> error_msgs;
        Parser(std::shared_ptr<TokenStream> tokStream) : tokStream(tokStream) {}
        auto Parse() -> std::unique_ptr<AST::ProgramAST>;
        auto hasErrors() -> bool { return !error_msgs.empty(); }
    };
}
