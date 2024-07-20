#include "Ast.h"
#include "Lexer.h"
namespace sammine_lang {
class Parser {
private:
  std::shared_ptr<TokenStream> tokStream;
  [[nodiscard]]
  auto ParseProgram() -> std::unique_ptr<AST::ProgramAST>;

  // Parse definition
  [[nodiscard]]
  auto ParseDefinition() -> std::unique_ptr<AST::DefinitionAST>;
  [[nodiscard]]
  auto ParsePrototype() -> std::unique_ptr<AST::PrototypeAST>;
  [[nodiscard]]
  auto ParseFuncDef() -> std::unique_ptr<AST::DefinitionAST>;
  [[nodiscard]]
  auto ParseVarDef() -> std::unique_ptr<AST::DefinitionAST>;

  // Parse type
  [[nodiscard]]
  auto ParseTypedVar() -> std::unique_ptr<AST::TypedVarAST>;

  // Parse expressions
  [[nodiscard]]
  auto ParseExpr() -> std::unique_ptr<AST::ExprAST>;
  [[nodiscard]]
  auto ParsePrimaryExpr() -> std::unique_ptr<AST::ExprAST>;
  [[nodiscard]]
  auto ParseBinaryExpr(int prededence, std::unique_ptr<AST::ExprAST> LHS)
      -> std::unique_ptr<AST::ExprAST>;
  [[nodiscard]]
  auto ParseCallExpr() -> std::unique_ptr<AST::ExprAST>;
  [[nodiscard]]
  auto ParseArguments()
      -> std::unique_ptr<std::vector<std::unique_ptr<AST::ExprAST>>>;
  [[nodiscard]]
  auto ParseNumberExpr() -> std::unique_ptr<AST::ExprAST>;
  [[nodiscard]]
  auto ParseVariableExpr() -> std::unique_ptr<AST::ExprAST>;

  // Parse block
  [[nodiscard]]
  auto ParseBlock() -> std::unique_ptr<AST::BlockAST>;

  // Parse parameters
  [[nodiscard]]
  auto ParseParams()
      -> std::unique_ptr<std::vector<std::unique_ptr<AST::TypedVarAST>>>;

  // Utilities
  [[nodiscard]]
  auto expect(TokenType tokType, bool exhausts = false,
              TokenType until = TokenType::TokEOF,
              const std::string &message = "") -> std::shared_ptr<Token>;

public:
  std::vector<std::string> error_msgs;
  [[nodiscard]]
  Parser() {}
  [[nodiscard]]
  Parser(std::shared_ptr<TokenStream> tokStream)
      : tokStream(tokStream) {}
  [[nodiscard]]
  auto Parse() -> std::unique_ptr<AST::ProgramAST>;
  [[nodiscard]]
  auto hasErrors() -> bool {
    return !error_msgs.empty();
  }
  void log_error(const std::string &message);
};
} // namespace sammine_lang
