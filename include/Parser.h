#pragma once
#include "Ast.h"
#include "Lexer.h"
#include "Utilities.h"
#include "tl/expected.hpp"
namespace sammine_lang {
enum ParserError {
  COMMITTED,
  NONCOMMITTED,
};
class Parser {
public:
  std::shared_ptr<TokenStream> tokStream;
  [[nodiscard]]
  auto
  ParseProgram() -> tl::expected<std::shared_ptr<AST::ProgramAST>, ParserError>;

  // Parse definition
  [[nodiscard]]
  auto ParseDefinition()
      -> tl::expected<std::shared_ptr<AST::DefinitionAST>, ParserError>;
  [[nodiscard]]
  auto ParsePrototype()
      -> tl::expected<std::shared_ptr<AST::PrototypeAST>, ParserError>;
  [[nodiscard]]
  auto ParseFuncDef()
      -> tl::expected<std::shared_ptr<AST::DefinitionAST>, ParserError>;
  [[nodiscard]]
  auto
  ParseVarDef() -> tl::expected<std::shared_ptr<AST::ExprAST>, ParserError>;

  // Parse type
  [[nodiscard]]
  auto ParseTypedVar()
      -> tl::expected<std::shared_ptr<AST::TypedVarAST>, ParserError>;
  [[nodiscard]]

  // Parse expressions
  [[nodiscard]]
  auto ParseExpr() -> tl::expected<std::shared_ptr<AST::ExprAST>, ParserError>;
  [[nodiscard]]
  auto ParsePrimaryExpr()
      -> tl::expected<std::shared_ptr<AST::ExprAST>, ParserError>;
  [[nodiscard]]
  auto ParseBinaryExpr(int prededence, std::shared_ptr<AST::ExprAST> LHS)
      -> tl::expected<std::shared_ptr<AST::ExprAST>, ParserError>;
  [[nodiscard]]
  auto
  ParseCallExpr() -> tl::expected<std::shared_ptr<AST::ExprAST>, ParserError>;
  [[nodiscard]]
  auto ParseArguments()
      -> tl::expected<
          std::shared_ptr<std::vector<std::shared_ptr<AST::ExprAST>>>,
          ParserError>;
  [[nodiscard]]
  auto
  ParseNumberExpr() -> tl::expected<std::shared_ptr<AST::ExprAST>, ParserError>;
  [[nodiscard]]
  auto ParseVariableExpr()
      -> tl::expected<std::shared_ptr<AST::ExprAST>, ParserError>;
  [[nodiscard]]

  // Parse block
  [[nodiscard]]
  auto
  ParseBlock() -> tl::expected<std::shared_ptr<AST::BlockAST>, ParserError>;
  [[nodiscard]]

  // Parse parameters
  [[nodiscard]]
  auto ParseParams()
      -> tl::expected<
          std::shared_ptr<std::vector<std::shared_ptr<AST::TypedVarAST>>>,
          ParserError>;

  // Utilities
  [[nodiscard]]
  auto expect(TokenType tokType, bool exhausts = false,
              TokenType until = TokenType::TokEOF,
              const std::string &message = "") -> std::shared_ptr<Token>;

  sammine_util::ErrorMsgs error_msgs;
  [[nodiscard]]
  Parser() {}
  [[nodiscard]]
  Parser(std::shared_ptr<TokenStream> tokStream)
      : tokStream(tokStream) {}
  [[nodiscard]]
  auto Parse() -> tl::expected<std::shared_ptr<AST::ProgramAST>, ParserError>;
  [[nodiscard]]
  auto hasErrors() -> bool {
    return !error_msgs.errors.empty();
  }
  void log_error(const std::string &message);
};
} // namespace sammine_lang
