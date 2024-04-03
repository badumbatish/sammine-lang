//
// Created by jjasmine on 3/7/24.
//

#ifndef SAMMINE_LANG_LEXER_H
#define SAMMINE_LANG_LEXER_H

#include <map>

//! \file Lexer.h
//! \brief Houses declaration (and some small implementation) of TokenType,
//! TokenMap, Token, TokenStream, Lexer

#include "FileRAII.h"
#include <memory>
#include <utility>
#include <vector>

namespace sammine_lang {
enum TokenType {
  // TODO: Add void keyword for return
  TokADD, // +
  TokSUB, // -
  TokMUL, // *
  TokDIV, // /
  TokMOD, // %

  TokAddAssign, // +=
  TokAddIncr,   // ++
  TokSubAssign, // -=
  TokSubDecr,   // --
  TokMulAssign, // *=
  TokDivAssign, // /=

  TokAND,        // &&
  TokAndLogical, // &
  TokOR,         // ||
  TokORLogical,  // |
  TokXOR,        // ^
  TokSHL,        // <<
  TokSHR,        // >>

  TokEQUAL,     // ==
  TokLESS,      // <
  TokLessEqual, // <=

  TokGREATER,      // >
  TokGreaterEqual, // >=

  TokASSIGN, // =
  TokNOT,    // !

  // TokEXP AND FloorDiv
  TokEXP,      // **
  TokFloorDiv, // /_
  TokCeilDiv,  // /^

  // TokPAREN
  TokLeftParen,  // (
  TokRightParen, // )
  TokLeftCurly,  // {
  TokRightCurly, // }

  // Comma and colons and all that
  TokComma,       // ,
  TokDot,         // .
  TokSemiColon,   // ;
  TokColon,       // :
  TokDoubleColon, // ::

  // TokFunction
  TokReturn,
  TokMain,
  TokFunc,  // fn
  TokArrow, // ->
  TokLet,   // let
  // TokID
  TokID, // Representing an identifier

  // TokNum
  TokNum, // Representing a number
  // TokIf
  TokIf,   // if
  TokElse, // else

  // TokCOMMENTS
  TokSingleComment, //
  TokEOF,
  TokINVALID,
};

static const std::map<TokenType, std::string> TokenMap = {
    {TokADD, "+"},
    {TokSUB, "-"},
    {TokMUL, "*"},
    {TokDIV, "/"},
    {TokMOD, "%"},

    {TokAddAssign, "+="},
    {TokAddIncr, "++"},
    {TokSubAssign, "-="},
    {TokSubDecr, "--"},
    {TokMulAssign, "*="},
    {TokDivAssign, "/="},

    {TokAND, "&&"},
    {TokAndLogical, "&"},
    {TokOR, "||"},
    {TokORLogical, "|"},
    {TokXOR, "^"},
    {TokSHL, ">>"},
    {TokSHR, "<<"},
    {TokEQUAL, "=="},
    {TokLESS, "<"},
    {TokLessEqual, "<="},

    {TokGREATER, ">"},
    {TokGreaterEqual, ">="},

    {TokASSIGN, "="},
    {TokNOT, "!"},
    {TokEXP, "**"},
    {TokFloorDiv, "/_"},
    {TokCeilDiv, "/^"},

    {TokLeftParen, "("},
    {TokRightParen, ")"},
    {TokLeftCurly, "{"},
    {TokRightCurly, "}"},

    {TokComma, ","},
    {TokDot, "."},
    {TokSemiColon, ";"},
    {TokColon, ":"},
    {TokDoubleColon, "::"},

    // TokFunction
    {TokReturn, "return"},
    {TokMain, "main"},
    {TokFunc, "fn"},
    {TokArrow, "->"},
    {TokLet, "let"},
    {TokID, "identifier"},

    {TokNum, "number"},

    {TokIf, "if"},
    {TokElse, "else"},

    // TokCOMMENTS
    {TokSingleComment, "#"},
    {TokEOF, "EOF"},
    {TokINVALID, "UNRECOGNIZED"},
};

//! A class representing a location for sammine-lang, this is helpful in
//! debugging

//! .
//! .
class Location {
public:
  size_t line;
  size_t column;
  Location() : line(0), column(0) {}

  inline void advance() { column++; }
  inline void devance() {
    if (column == 0)
      return;
    else
      column--;
  }
  inline void newLine() {
    line++;
    column = 0;
  }

  friend std::ostream &operator<<(std::ostream &out, const Location &loc) {
    out << loc.line << ":"
        << loc.column;
    return out;
  }

  std::string to_string() const {
    return std::to_string(this->line) + ":" +
           std::to_string(this->column);
  }
};
//! A class representing a token for sammine-lang, includes TokenType, lexeme
//! and position pair as its members.

//! .
//! .
class Token {
public:
  TokenType type;
  std::string lexeme;
  Location location;
  Token(TokenType type, std::string lexeme, Location location)
      : type(type), lexeme(std::move(lexeme)), location(location){};
};

//! A helper class for Lexer to simplify the process of getting a token.

//!
//!
class TokenStream {
  std::vector<std::shared_ptr<Token>> TokStream;
  size_t i;
  bool error;

public:
  std::vector<std::shared_ptr<Token>> ErrStream;

  TokenStream() : i(0), TokStream(), error(false) {}

  void push_back(const std::shared_ptr<Token> &token) {
    if (token->type == TokINVALID) {
      error = true;
      ErrStream.push_back(token);
    } else {
      TokStream.push_back(token);
    }
  }

  bool hasErrors() { return error; }

  void push_back(const Token &token) {
    this->push_back(std::make_shared<Token>(token));
  }

  std::shared_ptr<Token> &exhaust_until(TokenType tokType) {
    if (tokType == TokenType::TokEOF) {
      i = TokStream.size() - 1;
      return TokStream.back();
    }
    while (!isEnd()) {
      if (TokStream[i]->type == tokType)
        return TokStream[i];
      else
        i++;
    }

    return TokStream.back();
  }

  bool isEnd() { return i >= (TokStream.size() - 1); }
  std::shared_ptr<Token> peek() { return TokStream[i]; };
  std::shared_ptr<Token> consume() {
    auto token = peek();
    i = std::min(TokStream.size() - 1, i + 1);
    return token;
  }

  Location currentLocation() {
    if (!TokStream.empty()) {
      return TokStream.front()->location;
    } else {
      return {};
    }
    return {};
  }
};
//! A lexer for sammine-lang

//! Not sure what to put in here
//! Not sure what to put in here
class Lexer {
private:
  Location location;
  std::shared_ptr<TokenStream> tokStream;

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
#endif // SAMMINE_LANG_LEXER_H
