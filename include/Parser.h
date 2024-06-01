#include "Ast.h"
#include "Lexer.h"
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
  auto ParsePrimaryExpr() -> std::unique_ptr<AST::ExprAST>;
  auto ParseBinaryExpr(int prededence, std::unique_ptr<AST::ExprAST> LHS)
      -> std::unique_ptr<AST::ExprAST>;
  auto ParseCallExpr() -> std::unique_ptr<AST::ExprAST>;
  auto ParseArguments()
      -> std::unique_ptr<std::vector<std::unique_ptr<AST::ExprAST>>>;
  auto ParseNumberExpr() -> std::unique_ptr<AST::ExprAST>;
  auto ParseVariableExpr() -> std::unique_ptr<AST::ExprAST>;

  // Parse block
  auto ParseBlock() -> std::unique_ptr<AST::BlockAST>;

  // Parse parameters
  auto ParseParams()
      -> std::unique_ptr<std::vector<std::unique_ptr<AST::TypedVarAST>>>;

  // Utilities
  auto expect(TokenType tokType, bool exhausts = false,
              TokenType until = TokenType::TokEOF,
              const std::string &message = "") -> std::shared_ptr<Token>;

public:
  std::vector<std::string> error_msgs;
  Parser() {}
  Parser(std::shared_ptr<TokenStream> tokStream) : tokStream(tokStream) {}
  auto Parse() -> std::unique_ptr<AST::ProgramAST>;
  auto hasErrors() -> bool { return !error_msgs.empty(); }
  void log_error(const std::string &message);
};
} // namespace sammine_lang
