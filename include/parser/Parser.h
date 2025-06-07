#pragma once
#include "ast/Ast.h"
#include "lex/Token.h"
#include "util/Utilities.h"
#include <memory>
#include <optional>
#include <utility>
namespace sammine_lang {
enum ParserError {
  SUCCESS,
  COMMITTED_NO_MORE_ERROR,
  COMMITTED_EMIT_MORE_ERROR,
  NONCOMMITTED,
};
using namespace AST;
using namespace sammine_util;
class Parser : public Reportee {

  void error(const std::string &msg, Location loc = Location(-1, -1)) {
    if (reporter.has_value()) {
      reporter->get().immediate_error(msg, loc);
    }
    has_error = true;
  }
  void diag(const std::string &msg, Location loc = Location(-1, -1)) {
    if (reporter.has_value()) {
      reporter->get().immediate_diag(msg, loc);
    }
  }
  bool has_error = false;

public:
  template <class T> using p = std::pair<std::unique_ptr<T>, ParserError>;
  template <class T> using u = std::unique_ptr<T>;
  virtual bool has_errors() const override { return this->has_error; }
  std::optional<std::reference_wrapper<Reporter>> reporter;
  std::shared_ptr<TokenStream> tokStream;
  [[nodiscard]]
  auto ParseProgram() -> u<ProgramAST>;

  // Parse definition
  [[nodiscard]]
  auto ParseDefinition() -> p<DefinitionAST>;
  [[nodiscard]]
  auto ParsePrototype() -> p<PrototypeAST>;
  [[nodiscard]]
  auto ParseFuncDef() -> p<DefinitionAST>;
  [[nodiscard]]
  auto ParseVarDef() -> p<ExprAST>;

  // Parse type
  [[nodiscard]]
  auto ParseTypedVar() -> p<TypedVarAST>;
  [[nodiscard]]

  // Parse pressions
  [[nodiscard]]
  auto ParseExpr() -> p<ExprAST>;
  [[nodiscard]]
  auto ParsePrimaryExpr() -> p<ExprAST>;
  [[nodiscard]]
  auto ParseBinaryExpr(int prededence, u<ExprAST> LHS) -> p<ExprAST>;
  [[nodiscard]]
  auto ParseBoolExpr() -> p<ExprAST>;

  [[nodiscard]] auto ParseCallExpr() -> p<ExprAST>;
  [[nodiscard]] auto ParseReturnExpr() -> p<ExprAST>;
  [[nodiscard]]
  auto ParseArguments() -> std::pair<std::vector<u<ExprAST>>, ParserError>;
  [[nodiscard]]
  auto ParseIfExpr() -> p<ExprAST>;
  [[nodiscard]]
  auto ParseNumberExpr() -> p<ExprAST>;
  [[nodiscard]]
  auto ParseStringExpr() -> p<ExprAST>;
  [[nodiscard]]
  auto ParseVariableExpr() -> p<ExprAST>;
  [[nodiscard]]

  // Parse block
  [[nodiscard]]
  auto ParseBlock() -> p<BlockAST>;
  [[nodiscard]]

  // Parse parameters
  [[nodiscard]]
  auto ParseParams() -> std::pair<std::vector<u<TypedVarAST>>, ParserError>;

  // Utilities
  [[nodiscard]]
  auto expect(TokenType tokType, bool exhausts = false,
              TokenType until = TokenType::TokEOF,
              const std::string &message = "") -> std::shared_ptr<Token>;

  [[nodiscard]]
  Parser(
      std::optional<std::reference_wrapper<Reporter>> reporter = std::nullopt)
      : reporter(reporter) {}
  [[nodiscard]]
  Parser(
      std::shared_ptr<TokenStream> tokStream,
      std::optional<std::reference_wrapper<Reporter>> reporter = std::nullopt)
      : reporter(reporter), tokStream(tokStream) {}
  [[nodiscard]]

  [[nodiscard]]
  auto Parse() -> u<ProgramAST>;
};
} // namespace sammine_lang
