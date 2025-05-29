#pragma once
#include "Ast.h"
#include "Lexer.h"
#include "Utilities.h"
#include "tl/expected.hpp"
namespace sammine_lang {
enum ParserError {
  COMMITTED_NO_MORE_ERROR,
  COMMITTED_EMIT_MORE_ERROR,
  NONCOMMITTED,
};
class Parser : public sammine_util::Reportee {
private:
  void report(const std::string &msg) {
    if (show_diagnostics)
      std::cout << msg << std::endl;
  }

public:
  bool show_diagnostics;
  std::shared_ptr<TokenStream> tokStream;
  [[nodiscard]]
  auto ParseProgram()
      -> tl::expected<std::unique_ptr<AST::ProgramAST>, ParserError>;

  // Parse definition
  [[nodiscard]]
  auto ParseDefinition()
      -> tl::expected<std::unique_ptr<AST::DefinitionAST>, ParserError>;
  [[nodiscard]]
  auto ParsePrototype()
      -> tl::expected<std::unique_ptr<AST::PrototypeAST>, ParserError>;
  [[nodiscard]]
  auto ParseFuncDef()
      -> tl::expected<std::unique_ptr<AST::DefinitionAST>, ParserError>;
  [[nodiscard]]
  auto ParseVarDef()
      -> tl::expected<std::unique_ptr<AST::ExprAST>, ParserError>;

  // Parse type
  [[nodiscard]]
  auto ParseTypedVar()
      -> tl::expected<std::unique_ptr<AST::TypedVarAST>, ParserError>;
  [[nodiscard]]

  // Parse expressions
  [[nodiscard]]
  auto ParseExpr() -> tl::expected<std::unique_ptr<AST::ExprAST>, ParserError>;
  [[nodiscard]]
  auto ParsePrimaryExpr()
      -> tl::expected<std::unique_ptr<AST::ExprAST>, ParserError>;
  [[nodiscard]]
  auto ParseBinaryExpr(int prededence, std::unique_ptr<AST::ExprAST> LHS)
      -> tl::expected<std::unique_ptr<AST::ExprAST>, ParserError>;
  [[nodiscard]]
  auto ParseBoolExpr()
      -> tl::expected<std::unique_ptr<AST::ExprAST>, ParserError>;

  [[nodiscard]] auto ParseCallExpr()
      -> tl::expected<std::unique_ptr<AST::ExprAST>, ParserError>;
  [[nodiscard]] auto ParseReturnExpr()
      -> tl::expected<std::unique_ptr<AST::ExprAST>, ParserError>;
  [[nodiscard]]
  auto ParseArguments()
      -> tl::expected<std::vector<std::unique_ptr<AST::ExprAST>>, ParserError>;
  [[nodiscard]]
  auto ParseIfExpr()
      -> tl::expected<std::unique_ptr<AST::ExprAST>, ParserError>;
  [[nodiscard]]
  auto ParseElseExpr()
      -> tl::expected<std::unique_ptr<AST::ExprAST>, ParserError>;
  [[nodiscard]]
  auto ParseNumberExpr()
      -> tl::expected<std::unique_ptr<AST::ExprAST>, ParserError>;
  [[nodiscard]]
  auto ParseVariableExpr()
      -> tl::expected<std::unique_ptr<AST::ExprAST>, ParserError>;
  [[nodiscard]]

  // Parse block
  [[nodiscard]]
  auto ParseBlock()
      -> tl::expected<std::unique_ptr<AST::BlockAST>, ParserError>;
  [[nodiscard]]

  // Parse parameters
  [[nodiscard]]
  auto ParseParams()
      -> tl::expected<std::vector<std::unique_ptr<AST::TypedVarAST>>,
                      ParserError>;

  // Utilities
  [[nodiscard]]
  auto expect(TokenType tokType, bool exhausts = false,
              TokenType until = TokenType::TokEOF,
              const std::string &message = "") -> std::shared_ptr<Token>;

  [[nodiscard]]
  Parser(bool diagnostics = false)
      : show_diagnostics(diagnostics) {}
  [[nodiscard]]
  Parser(std::shared_ptr<TokenStream> tokStream, bool diagnostics = false)
      : show_diagnostics(diagnostics), tokStream(tokStream) {}
  [[nodiscard]]

  [[nodiscard]]
  auto Parse() -> tl::expected<std::unique_ptr<AST::ProgramAST>, ParserError>;
};
} // namespace sammine_lang
