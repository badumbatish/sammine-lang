
#include "Parser.h"
#include "Ast.h"
#include "Lexer.h"
#include "Utilities.h"
#include "tl/expected.hpp"
#include <cstdlib>
#include <functional>
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
    -> tl::expected<std::shared_ptr<AST::ProgramAST>, ParserError> {
  return ParseProgram();
}

auto Parser::ParseProgram()
    -> tl::expected<std::shared_ptr<AST::ProgramAST>, ParserError> {
  auto programAST = std::make_shared<AST::ProgramAST>();
  while (!tokStream->isEnd()) {
    auto def_result = ParseDefinition();
    if (def_result) {
      programAST->DefinitionVec.push_back(std::move(def_result.value()));
    }
  }

  return programAST;
}

auto Parser::ParseDefinition()
    -> tl::expected<std::shared_ptr<AST::DefinitionAST>, ParserError> {
  using ParseFunction = std::function<
      tl::expected<std::shared_ptr<AST::DefinitionAST>, ParserError>(Parser *)>;
  std::vector<std::pair<ParseFunction, bool>> ParseFunctions = {
      {&Parser::ParseFuncDef, false},
  };

  for (auto [fn, required] : ParseFunctions) {
    auto result = fn(this);
    if (result)
      return result;
  }

  auto result = expect(TokenType::TokINVALID, true, TokEOF,
                       "Failed to parse any meaningful definitions");
  return tl::make_unexpected(ParserError::NONCOMMITTED);
}

auto Parser::ParseFuncDef()
    -> tl::expected<std::shared_ptr<AST::DefinitionAST>, ParserError> {
  // this is for extern
  auto extern_fn = expect(TokenType::TokExtern);
  if (extern_fn) {

    auto prototype = ParsePrototype();
    if (!prototype) {
      sammine_util::abort("abort extern");
      auto result = expect(TokenType::TokINVALID, true, TokRightCurly,
                           "Failed to parse a prototype of a function");
      return tl::make_unexpected(ParserError::COMMITTED);
    }

    auto semi_colon = expect(TokenType::TokSemiColon);
    if (!semi_colon)
      sammine_util::abort("Need semicolon when parsing extern");
    return std::make_shared<AST::ExternAST>(prototype.value());
  }

  // this is for fn
  auto fn = expect(TokenType::TokFunc);
  if (!fn)
    return tl::make_unexpected(ParserError::NONCOMMITTED);

  auto prototype = ParsePrototype();
  if (!prototype) {
    auto result = expect(TokenType::TokINVALID, true, TokRightCurly,
                         "Failed to parse a prototype of a function");
    return tl::make_unexpected(ParserError::COMMITTED);
  }

  auto block = ParseBlock();
  if (!block) {
    auto result = expect(TokenType::TokINVALID, true, TokRightCurly,
                         "Failed to parse a block of a function definition");
    return tl::make_unexpected(ParserError::COMMITTED);
  }

  return std::make_shared<AST::FuncDefAST>(std::move(prototype.value()),
                                           std::move(block.value()));
}

//! Parsing implementation for a variable decl/def

//! Accepts a let, continue parsing inside and (enable error reporting if
//! possible). If a `let` is not found then return a nullptr.
auto Parser::ParseVarDef()
    -> tl::expected<std::shared_ptr<AST::ExprAST>, ParserError> {
  auto let = expect(TokenType::TokLet);
  if (!let)
    return tl::make_unexpected(ParserError::NONCOMMITTED);
  auto typedVar = ParseTypedVar();
  if (!typedVar) {
    auto result = expect(TokenType::TokINVALID, true, TokSemiColon,
                         "Failed to parse typed variable");
    return tl::make_unexpected(ParserError::COMMITTED);
  }

  auto assign = expect(TokenType::TokASSIGN, true, TokSemiColon,
                       "Failed to match assign token `=`");
  if (!assign)
    return tl::make_unexpected(ParserError::COMMITTED);

  auto expr = ParseExpr();
  if (!expr) {
    auto result = expect(TokenType::TokINVALID, true, TokSemiColon,
                         "Failed to parse expression");
    return tl::make_unexpected(ParserError::COMMITTED);
  }

  auto semicolon = expect(TokenType::TokSemiColon, true, TokSemiColon,
                          "Failed to match semicolon token `;`");

  auto varDef = std::make_shared<AST::VarDefAST>(std::move(typedVar.value()),
                                                 std::move(expr.value()));

  return varDef;
}

auto Parser::ParseTypedVar()
    -> tl::expected<std::shared_ptr<AST::TypedVarAST>, ParserError> {
  auto name = expect(TokenType::TokID);
  if (!name)
    return tl::make_unexpected(ParserError::NONCOMMITTED);
  auto colon = expect(TokenType::TokColon);
  if (!colon)
    return tl::make_unexpected(ParserError::COMMITTED);
  auto type = expect(TokenType::TokID);
  if (!type)
    return tl::make_unexpected(ParserError::COMMITTED);

  auto typedVar = std::make_shared<AST::TypedVarAST>();

  typedVar->name = name->lexeme;
  typedVar->type = type->lexeme;
  return typedVar;
}
auto Parser::ParsePrimaryExpr()
    -> tl::expected<std::shared_ptr<AST::ExprAST>, ParserError> {
  using ParseFunction =
      std::function<tl::expected<std::shared_ptr<AST::ExprAST>, ParserError>(
          Parser *)>;
  std::vector<ParseFunction> ParseFunctions = {
      &Parser::ParseCallExpr,
      &Parser::ParseNumberExpr,
      &Parser::ParseVariableExpr,
  };

  for (auto fn : ParseFunctions) {
    auto result = fn(this);
    if (result)
      return result;
    else if (!result && result.error() == ParserError::COMMITTED)
      return result;
  }

  return tl::make_unexpected(ParserError::NONCOMMITTED);
}
auto Parser::ParseExpr()
    -> tl::expected<std::shared_ptr<AST::ExprAST>, ParserError> {
  auto LHS = ParsePrimaryExpr();
  if (!LHS) {
    return LHS;
  }

  return ParseBinaryExpr(0, std::move(LHS.value()));
}

auto Parser::ParseBinaryExpr(int prededence, std::shared_ptr<AST::ExprAST> LHS)
    -> tl::expected<std::shared_ptr<AST::ExprAST>, ParserError> {
  while (true) {
    int TokPrec = GetTokPrecedence(tokStream->peek()->type);

    if (TokPrec < prededence)
      return LHS;

    auto binOpToken = tokStream->consume();

    auto RHS = ParsePrimaryExpr();
    if (!RHS) {
      return RHS;
    }

    int NextPrec = GetTokPrecedence(tokStream->peek()->type);
    if (TokPrec < NextPrec) {
      RHS = ParseBinaryExpr(TokPrec + 1, std::move(RHS.value()));
      if (!RHS) {
        return RHS;
      }
    }

    LHS = std::make_shared<AST::BinaryExprAST>(binOpToken, std::move(LHS),
                                               std::move(RHS.value()));
  }
}

auto Parser::ParseCallExpr()
    -> tl::expected<std::shared_ptr<AST::ExprAST>, ParserError> {

  auto id = expect(TokenType::TokID);
  if (id == nullptr)
    return tl::make_unexpected(ParserError::NONCOMMITTED);

  auto args = ParseArguments();
  if (args)
    return std::make_shared<AST::CallExprAST>(id->lexeme,
                                              std::move(args.value()));
  else {
    return std::make_shared<AST::VariableExprAST>(id->lexeme);
  }

  return tl::make_unexpected(ParserError::COMMITTED);
}

auto Parser::ParseNumberExpr()
    -> tl::expected<std::shared_ptr<AST::ExprAST>, ParserError> {
  auto numberExpr = std::make_shared<AST::NumberExprAST>();

  auto numberToken = expect(TokenType::TokNum);

  if (numberToken == nullptr) {
    return tl::make_unexpected(ParserError::NONCOMMITTED);
  } else {
    numberExpr->number = numberToken->lexeme;
    return numberExpr;
  }
}

auto Parser::ParseVariableExpr()
    -> tl::expected<std::shared_ptr<AST::ExprAST>, ParserError> {
  auto name = expect(TokenType::TokID);

  if (name)
    return std::make_shared<AST::VariableExprAST>(name);
  return tl::make_unexpected(ParserError::NONCOMMITTED);
}

auto Parser::ParsePrototype()
    -> tl::expected<std::shared_ptr<AST::PrototypeAST>, ParserError> {
  auto id = expect(TokID);
  if (!id)
    return tl::make_unexpected(ParserError::NONCOMMITTED);

  auto params = ParseParams();
  if (!params)
    return tl::make_unexpected(params.error());
  auto arrow = expect(TokArrow);
  if (!arrow)
    return std::make_shared<AST::PrototypeAST>(id->lexeme, "",
                                               std::move(params.value()));
  ;

  auto returnType = expect(TokID);

  return std::make_shared<AST::PrototypeAST>(
      id->lexeme, returnType ? returnType->lexeme : "",
      std::move(params.value()));
}

auto Parser::ParseBlock()
    -> tl::expected<std::shared_ptr<AST::BlockAST>, ParserError> {

  auto leftCurly = expect(TokLeftCurly);
  if (!leftCurly)
    return tl::make_unexpected(ParserError::COMMITTED);
  // TODO : Cannot just parse a return stmt.
  // TODO : We need to also parse other statement as well

  auto blockAST = std::make_shared<AST::BlockAST>();
  while (true) {
    auto a = ParseExpr();
    if (!a && a.error() == ParserError::NONCOMMITTED) {
    }
    if (!a && a.error() == ParserError::COMMITTED)
      return tl::make_unexpected(a.error());
    else if (a) {
      blockAST->Statements.push_back(a.value());
      auto semi = expect(TokenType::TokSemiColon);
      if (!semi)
        return tl::make_unexpected(ParserError::COMMITTED);

      continue;
    }

    auto b = ParseVarDef();
    if (!b && b.error() == ParserError::NONCOMMITTED) {
      break;
    }
    if (!b && b.error() == ParserError::COMMITTED)
      return tl::make_unexpected(b.error());
    else if (b) {
      blockAST->Statements.push_back(b.value());
    }
  }

  auto rightCurly = expect(TokRightCurly);

  if (!rightCurly)
    return tl::make_unexpected(ParserError::COMMITTED);

  return blockAST;
}

// Parsing of parameters in a function call, we use leftParen and rightParen as
// appropriate stopping point
auto Parser::ParseParams()
    -> tl::expected<
        std::shared_ptr<std::vector<std::shared_ptr<AST::TypedVarAST>>>,
        ParserError> {
  auto leftParen = expect(TokLeftParen);
  if (leftParen == nullptr)
    return tl::make_unexpected(ParserError::NONCOMMITTED);

  // COMMITMENT POINT
  std::shared_ptr<std::vector<std::shared_ptr<AST::TypedVarAST>>> vec =
      std::make_shared<std::vector<std::shared_ptr<AST::TypedVarAST>>>();
  auto typeVar = ParseTypedVar();
  if (typeVar) {
    vec->push_back(std::move(typeVar.value()));
  }
  while (true) {
    auto comma = expect(TokComma);
    if (comma == nullptr)
      break;

    // Report error if we find comma but cannot find typeVar
    typeVar = ParseTypedVar();
    if (!typeVar) {
      log_error("Failed to find typed variable after comma");
      return tl::make_unexpected(typeVar.error());
    } else {
      vec->push_back(std::move(typeVar.value()));
    }
  }
  auto rightParen = expect(TokRightParen);
  if (rightParen == nullptr) {
    log_error(
        "Failed to find right parenthesis after processing [typed variables]");
    return tl::make_unexpected(ParserError::COMMITTED);
  }

  return vec;
}

auto Parser::ParseArguments()
    -> tl::expected<std::shared_ptr<std::vector<std::shared_ptr<AST::ExprAST>>>,
                    ParserError> {
  auto leftParen = expect(TokLeftParen);
  if (leftParen == nullptr)
    return tl::make_unexpected(ParserError::NONCOMMITTED);

  std::shared_ptr<std::vector<std::shared_ptr<AST::ExprAST>>> vec =
      std::make_shared<std::vector<std::shared_ptr<AST::ExprAST>>>();

  auto expr = ParseExpr();
  if (expr) {
    vec->push_back(std::move(expr.value()));
  }
  while (true) {
    /*auto typeVar = ParseTypedVar();*/
    auto comma = expect(TokComma);
    if (comma == nullptr)
      break;

    expr = ParseExpr();
    if (!expr)
      return tl::make_unexpected(ParserError::COMMITTED);
  }
  auto rightParen = expect(TokRightParen);
  if (!rightParen)
    return tl::make_unexpected(ParserError::COMMITTED);

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
    if (exhausts) {
      log_error(message);
      tokStream->exhaust_until(until);
    }

    return nullptr;
  }
}

auto Parser::log_error(const std::string &message) -> void {
  error_msgs.errors.push_back({tokStream->currentLocation(), message});
}
} // namespace sammine_lang
