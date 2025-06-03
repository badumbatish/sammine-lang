//
// Created by jjasmine on 3/7/24.
//

#pragma once
#include "fmt/format.h"
#include "lex/Token.h"
#include "util/Utilities.h"
#include <algorithm>
#include <cassert>
#include <map>

//! \file Lexer.h
//! \brief Houses declaration (and some small implementation) of TokenType,
//! TokenMap, Token, TokenStream, Lexer

#include <memory>

namespace sammine_lang {
//! A lexer for sammine-lang

//! Not sure what to put in here
//! Not sure what to put in here
class Lexer : public sammine_util::Reportee {
private:
  sammine_util::Location location;
  std::shared_ptr<TokenStream> tokStream;

  [[nodiscard]]
  size_t handleNumber(size_t i, const std::string &input);
  size_t handleSpaces(size_t i, const std::string &input);
  size_t handleID(size_t i, const std::string &input);
  size_t handleInvalid(size_t i, const std::string &input);
  size_t handleOperators(size_t i, const std::string &input);
  size_t handleOperatorsADD(size_t i, const std::string &input);
  size_t handleOperatorsSUB(size_t i, const std::string &input);
  size_t handleOperatorsMUL(size_t i, const std::string &input);
  size_t handleOperatorsDIV(size_t i, const std::string &input);
  size_t handleOperatorsMOD(size_t i, const std::string &input);
  size_t handleOperatorsAND(size_t i, const std::string &input);
  size_t handleOperatorsOR(size_t i, const std::string &input);
  size_t handleOperatorsXOR(size_t i, const std::string &input);
  size_t handleOperatorsCompLeft(size_t i, const std::string &input);
  size_t handleOperatorsCompRight(size_t i, const std::string &input);
  size_t handleOperatorsEqual(size_t i, const std::string &input);
  size_t handleOperatorsNot(size_t i, const std::string &input);

  size_t handleUtility(size_t i, const std::string &input);
  size_t handleUtilityPAREN(size_t i, const std::string &input);
  size_t handleUtilityCURLY(size_t i, const std::string &input);
  size_t handleUtilityCOMMENT(size_t i, const std::string &input);
  size_t handleUtilityCOMMA(size_t i, const std::string &input);
  size_t handleUtilitySemiColon(size_t i, const std::string &input);
  size_t handleUtilityCOLON(size_t i, const std::string &input);

  void updateLocation();

public:
  explicit Lexer(const std::string &input);
  Lexer() : location() { tokStream = std::make_shared<TokenStream>(); }
  std::shared_ptr<Token> peek();
  std::shared_ptr<Token> consume();

  std::shared_ptr<TokenStream> getTokenStream() { return tokStream; }

  size_t advance(size_t i);

  size_t devance(size_t i);
};
} // namespace sammine_lang
