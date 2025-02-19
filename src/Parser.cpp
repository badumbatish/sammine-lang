
#include "Parser.h"
#include "Ast.h"
#include "Lexer.h"
#include "Utilities.h"
#include "tl/expected.hpp"
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>

namespace sammine_lang {

static std::map<TokenType, int> binopPrecedence = {{TokenType::TokASSIGN, 2},
                                                   {TokenType::TokLESS, 10},
                                                   {TokenType::TokADD, 20},
                                                   {TokenType::TokSUB, 20},
                                                   {TokenType::TokMUL, 40}};

int GetTokPrecedence(TokenType tokType) {
  int TokPrec = binopPrecedence[tokType];
  if (TokPrec <= 0)
    return -1;
  return TokPrec;
}

auto Parser::Parse()
    -> tl::expected<std::unique_ptr<AST::ProgramAST>, ParserError> {
  return ParseProgram();
}

auto Parser::ParseProgram()
    -> tl::expected<std::unique_ptr<AST::ProgramAST>, ParserError> {
  auto programAST = std::make_unique<AST::ProgramAST>();
  while (!tokStream->isEnd()) {
    auto def_result = ParseDefinition();
    if (def_result) {
      programAST->DefinitionVec.push_back(std::move(def_result.value()));
    }
  }

  return programAST;
}

auto Parser::ParseDefinition()
    -> tl::expected<std::unique_ptr<AST::DefinitionAST>, ParserError> {
  using ParseFunction = std::function<
      tl::expected<std::unique_ptr<AST::DefinitionAST>, ParserError>(Parser *)>;
  std::vector<std::pair<ParseFunction, bool>> ParseFunctions = {
      {&Parser::ParseFuncDef, false},
  };

  ParserError pe = NONCOMMITTED;
  for (auto [fn, required] : ParseFunctions) {
    auto result = fn(this);
    if (result)
      return result;
    else if (!result)
      pe = result.error();
  }

  auto result = expect(TokenType::TokINVALID, true, TokEOF,
                       pe == ParserError::COMMITTED_NO_MORE_ERROR
                           ? ""
                           : "Failed to parse any meaningful definitions");
  return tl::make_unexpected(COMMITTED_NO_MORE_ERROR);
}

auto Parser::ParseFuncDef()
    -> tl::expected<std::unique_ptr<AST::DefinitionAST>, ParserError> {
  // this is for extern
  auto extern_fn = expect(TokenType::TokExtern);
  if (extern_fn) {

    auto prototype = ParsePrototype();
    if (!prototype) {
      sammine_util::abort("abort extern");
      auto result = expect(TokenType::TokINVALID, true, TokRightCurly,
                           "Failed to parse a prototype of a function");
      return tl::make_unexpected(ParserError::COMMITTED_NO_MORE_ERROR);
    }

    auto semi_colon = expect(TokenType::TokSemiColon);
    if (!semi_colon)
      sammine_util::abort("Need semicolon when parsing extern");
    return std::make_unique<AST::ExternAST>(std::move(prototype.value()));
  }

  // this is for fn
  auto fn = expect(TokenType::TokFunc);
  if (!fn)
    return tl::make_unexpected(ParserError::NONCOMMITTED);

  auto prototype = ParsePrototype();
  if (!prototype) {
    auto result = expect(TokenType::TokINVALID, true, TokRightCurly,
                         "Failed to parse a prototype of a function");
    return tl::make_unexpected(ParserError::COMMITTED_NO_MORE_ERROR);
  }

  auto block = ParseBlock();
  if (!block && block.error() == COMMITTED_NO_MORE_ERROR) {
    /*auto result = expect(TokenType::TokINVALID, true, TokRightCurly,*/
    /*                     "Failed to parse a block of a function
     * definition");*/
    return tl::make_unexpected(ParserError::COMMITTED_NO_MORE_ERROR);
  }

  if (!block && block.error() == COMMITTED_EMIT_MORE_ERROR) {
    sammine_util::abort(
        "Parsing a block should handle its own closing curly block");
  }
  if (!block)
    sammine_util::abort();

  return std::make_unique<AST::FuncDefAST>(std::move(prototype.value()),
                                           std::move(block.value()));
}

//! Parsing implementation for a variable decl/def

//! Accepts a let, continue parsing inside and (enable error reporting if
//! possible). If a `let` is not found then return a nullptr.
auto Parser::ParseVarDef()
    -> tl::expected<std::unique_ptr<AST::ExprAST>, ParserError> {
  auto let = expect(TokenType::TokLet);
  if (!let)
    return tl::make_unexpected(ParserError::NONCOMMITTED);
  auto typedVar = ParseTypedVar();
  if (!typedVar) {
    auto result = expect(TokenType::TokINVALID, true, TokSemiColon,
                         "Failed to parse typed variable");
    return tl::make_unexpected(ParserError::COMMITTED_NO_MORE_ERROR);
  }

  auto assign = expect(TokenType::TokASSIGN, true, TokSemiColon,
                       "Failed to match assign token `=`");
  if (!assign)
    return tl::make_unexpected(ParserError::COMMITTED_NO_MORE_ERROR);

  auto expr = ParseExpr();
  if (!expr) {
    auto result = expect(TokenType::TokINVALID, true, TokSemiColon,
                         "Failed to parse expression");
    return tl::make_unexpected(ParserError::COMMITTED_NO_MORE_ERROR);
  }

  auto semicolon = expect(TokenType::TokSemiColon, true, TokSemiColon,
                          "Failed to match semicolon token `;`");

  auto varDef = std::make_unique<AST::VarDefAST>(std::move(typedVar.value()),
                                                 std::move(expr.value()));

  return varDef;
}

auto Parser::ParseTypedVar()
    -> tl::expected<std::unique_ptr<AST::TypedVarAST>, ParserError> {
  auto name = expect(TokenType::TokID);
  if (!name)
    return tl::make_unexpected(ParserError::NONCOMMITTED);
  auto colon = expect(TokenType::TokColon);
  if (!colon)
    return tl::make_unexpected(ParserError::COMMITTED_NO_MORE_ERROR);
  auto type = expect(TokenType::TokID);
  if (!type)
    return tl::make_unexpected(ParserError::COMMITTED_NO_MORE_ERROR);

  return std::make_unique<AST::TypedVarAST>(name, type);
}
auto Parser::ParsePrimaryExpr()
    -> tl::expected<std::unique_ptr<AST::ExprAST>, ParserError> {
  using ParseFunction =
      std::function<tl::expected<std::unique_ptr<AST::ExprAST>, ParserError>(
          Parser *)>;
  std::vector<ParseFunction> ParseFunctions = {
      &Parser::ParseCallExpr,     &Parser::ParseIfExpr,
      &Parser::ParseNumberExpr,   &Parser::ParseBoolExpr,
      &Parser::ParseVariableExpr,
  };

  for (auto fn : ParseFunctions) {
    auto result = fn(this);
    if (result) {
      return result;
    } else if (!result &&
               result.error() == ParserError::COMMITTED_NO_MORE_ERROR) {
      sammine_util::abort("Failed to parse one of primary expr");
      return result;
    }
  }

  return tl::make_unexpected(ParserError::NONCOMMITTED);
}
auto Parser::ParseExpr()
    -> tl::expected<std::unique_ptr<AST::ExprAST>, ParserError> {
  auto LHS = ParsePrimaryExpr();
  if (!LHS) {
    return LHS;
  }

  auto next = ParseBinaryExpr(0, std::move(LHS.value()));
  if (!next && next.error() == COMMITTED_NO_MORE_ERROR) {
    auto result = expect(TokSemiColon, true, TokSemiColon, "");
    return tl::make_unexpected(COMMITTED_NO_MORE_ERROR);
  }
  if (!next && next.error() == COMMITTED_EMIT_MORE_ERROR)
    sammine_util::abort("ParseExpr should not handle error");

  return next;
}

auto Parser::ParseBinaryExpr(int prededence, std::unique_ptr<AST::ExprAST> LHS)
    -> tl::expected<std::unique_ptr<AST::ExprAST>, ParserError> {
  while (true) {
    int TokPrec = GetTokPrecedence(tokStream->peek()->type);

    if (TokPrec < prededence)
      return LHS;

    auto binOpToken = tokStream->consume();

    auto RHS = ParsePrimaryExpr();

    // We're committed here, so whether RHS is commited error or non-committed
    // error, we really should always return non-committed.
    //
    // If it is commited, we don't release any more report, just return RHS
    // like normal If it is commited, we add a report. Depend on programmer.
    if (!RHS && (RHS.error() == COMMITTED_EMIT_MORE_ERROR ||
                 RHS.error() == NONCOMMITTED)) {
      add_error(
          *binOpToken,
          fmt::format("Failed to parse the right-hand side of token `{}` in "
                      "binary expression",
                      binOpToken->lexeme));
      return tl::make_unexpected(COMMITTED_NO_MORE_ERROR);
    }
    if (!RHS && RHS.error() == COMMITTED_NO_MORE_ERROR) {
      return tl::make_unexpected(COMMITTED_NO_MORE_ERROR);
    }
    if (!RHS)
      sammine_util::abort();
    int NextPrec = GetTokPrecedence(tokStream->peek()->type);
    if (TokPrec < NextPrec) {
      RHS = ParseBinaryExpr(TokPrec + 1, std::move(RHS.value()));
      if (!RHS) {
        return RHS;
      }
    }

    LHS = std::make_unique<AST::BinaryExprAST>(binOpToken, std::move(LHS),
                                               std::move(RHS.value()));
  }
}

auto Parser::ParseCallExpr()
    -> tl::expected<std::unique_ptr<AST::ExprAST>, ParserError> {

  auto id = expect(TokenType::TokID);
  if (id == nullptr)
    return tl::make_unexpected(ParserError::NONCOMMITTED);

  auto args = ParseArguments();
  if (args)
    return std::make_unique<AST::CallExprAST>(id, std::move(args.value()));
  else {
    return std::make_unique<AST::VariableExprAST>(id);
  }

  return tl::make_unexpected(ParserError::COMMITTED_NO_MORE_ERROR);
}

auto Parser::ParseIfExpr()
    -> tl::expected<std::unique_ptr<AST::ExprAST>, ParserError> {
  auto if_tok = expect(TokenType::TokIf);

  if (!if_tok)
    return tl::make_unexpected(ParserError::NONCOMMITTED);

  auto cond = ParseExpr();

  if (!cond)
    return tl::make_unexpected(ParserError::COMMITTED_NO_MORE_ERROR);

  auto then_block = ParseBlock();

  if (!then_block)
    return tl::make_unexpected(ParserError::COMMITTED_NO_MORE_ERROR);

  auto else_tok = expect(TokenType::TokElse);
  if (!else_tok) {
    return std::make_unique<AST::IfExprAST>(std::move(cond.value()),
                                            std::move(then_block.value()),
                                            std::make_unique<AST::BlockAST>());
  }
  auto else_block = ParseBlock();
  if (!else_block)
    return tl::make_unexpected(ParserError::COMMITTED_NO_MORE_ERROR);

  return std::make_unique<AST::IfExprAST>(std::move(cond.value()),
                                          std::move(then_block.value()),
                                          std::move(else_block.value()));
}
auto Parser::ParseElseExpr()
    -> tl::expected<std::unique_ptr<AST::ExprAST>, ParserError> {
  return nullptr;
}
auto Parser::ParseNumberExpr()
    -> tl::expected<std::unique_ptr<AST::ExprAST>, ParserError> {

  if (auto numberToken = expect(TokenType::TokNum))
    return std::make_unique<AST::NumberExprAST>(numberToken);
  else
    return tl::make_unexpected(ParserError::NONCOMMITTED);
}

auto Parser::ParseBoolExpr()
    -> tl::expected<std::unique_ptr<AST::ExprAST>, ParserError> {

  if (auto true_tok = expect(TokenType::TokTrue)) {
    return std::make_unique<AST::BoolExprAST>(true, true_tok->location);
  }

  if (auto false_tok = expect(TokenType::TokFalse)) {
    return std::make_unique<AST::BoolExprAST>(false, false_tok->location);
  }

  return tl::make_unexpected(ParserError::NONCOMMITTED);
}
auto Parser::ParseVariableExpr()
    -> tl::expected<std::unique_ptr<AST::ExprAST>, ParserError> {
  if (auto name = expect(TokenType::TokID))
    return std::make_unique<AST::VariableExprAST>(name);
  return tl::make_unexpected(ParserError::NONCOMMITTED);
}

auto Parser::ParsePrototype()
    -> tl::expected<std::unique_ptr<AST::PrototypeAST>, ParserError> {
  auto id = expect(TokID);
  if (!id)
    return tl::make_unexpected(ParserError::NONCOMMITTED);

  auto params = ParseParams();
  if (!params)
    return tl::make_unexpected(params.error());
  auto arrow = expect(TokArrow);
  if (!arrow)
    return std::make_unique<AST::PrototypeAST>(id, std::move(params.value()));
  ;

  auto returnType = expect(TokID);

  // TODO: make sure we handle a return type here
  return std::make_unique<AST::PrototypeAST>(id, returnType,
                                             std::move(params.value()));
}

auto Parser::ParseBlock()
    -> tl::expected<std::unique_ptr<AST::BlockAST>, ParserError> {

  auto leftCurly = expect(TokLeftCurly);
  if (!leftCurly)
    return tl::make_unexpected(ParserError::COMMITTED_NO_MORE_ERROR);
  // TODO : Cannot just parse a return stmt.
  // TODO : We need to also parse other statement as well

  auto blockAST = std::make_unique<AST::BlockAST>();
  while (true) {
    auto a = ParseExpr();

    if (!a && a.error() == ParserError::COMMITTED_NO_MORE_ERROR)
      return tl::make_unexpected(a.error());
    if (a) {
      blockAST->Statements.push_back(std::move(a.value()));
      auto semi = expect(TokenType::TokSemiColon);
      if (!semi)
        return tl::make_unexpected(ParserError::COMMITTED_NO_MORE_ERROR);

      continue;
    }

    auto b = ParseVarDef();
    if (!b && b.error() == ParserError::NONCOMMITTED) {

      break;
    }
    if (!b && b.error() == ParserError::COMMITTED_NO_MORE_ERROR) {
      return tl::make_unexpected(b.error());
    }
    blockAST->Statements.push_back(std::move(b.value()));
  }

  auto rightCurly = expect(TokRightCurly, true, TokEOF, "");

  if (!rightCurly)
    return tl::make_unexpected(ParserError::COMMITTED_NO_MORE_ERROR);

  return blockAST;
}

// Parsing of parameters in a function call, we use leftParen and rightParen
// as appropriate stopping point
auto Parser::ParseParams()
    -> tl::expected<std::vector<std::unique_ptr<AST::TypedVarAST>>,
                    ParserError> {
  auto leftParen = expect(TokLeftParen);
  if (leftParen == nullptr)
    return tl::make_unexpected(ParserError::NONCOMMITTED);

  // COMMITMENT POINT
  auto vec = std::vector<std::unique_ptr<AST::TypedVarAST>>();
  auto typeVar = ParseTypedVar();
  if (typeVar) {
    vec.push_back(std::move(typeVar.value()));
  }
  while (true) {
    auto comma = expect(TokComma);
    if (comma == nullptr)
      break;

    // Report error if we find comma but cannot find typeVar
    typeVar = ParseTypedVar();
    if (!typeVar) {
      add_error(tokStream->currentLocation(),
                "Failed to find typed variable after comma");
      return tl::make_unexpected(typeVar.error());
    } else {
      vec.push_back(std::move(typeVar.value()));
    }
  }
  auto rightParen = expect(TokRightParen);
  if (rightParen == nullptr) {
    add_error(tokStream->currentLocation(),
              "Failed to find right parenthesis after processing [typed "
              "variables]");
    return tl::make_unexpected(ParserError::COMMITTED_NO_MORE_ERROR);
  }

  return vec;
}

auto Parser::ParseArguments()
    -> tl::expected<std::vector<std::unique_ptr<AST::ExprAST>>, ParserError> {
  auto leftParen = expect(TokLeftParen);
  if (leftParen == nullptr)
    return tl::make_unexpected(ParserError::NONCOMMITTED);

  auto vec = std::vector<std::unique_ptr<AST::ExprAST>>();

  auto expr = ParseExpr();
  if (expr) {
    vec.push_back(std::move(expr.value()));
  }
  while (true) {
    /*auto typeVar = ParseTypedVar();*/
    auto comma = expect(TokComma);
    if (comma == nullptr)
      break;

    expr = ParseExpr();
    if (!expr)
      return tl::make_unexpected(ParserError::COMMITTED_NO_MORE_ERROR);
  }
  auto rightParen = expect(TokRightParen);
  if (!rightParen)
    return tl::make_unexpected(ParserError::COMMITTED_NO_MORE_ERROR);

  return vec;
}

auto Parser::expect(TokenType tokType, bool exhausts, TokenType until,
                    const std::string &message) -> std::shared_ptr<Token> {
  auto currentToken = tokStream->peek();
  auto result = !tokStream->isEnd() && currentToken->type == tokType;
  if (result) {
    return tokStream->consume();
  } else {
    // TODO: Add error reporting after this point.
    if (!message.empty())
      add_error(tokStream->currentLocation(), message);
    if (exhausts)
      tokStream->exhaust_until(until);

    return nullptr;
  }
}

} // namespace sammine_lang
