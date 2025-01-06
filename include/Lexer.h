//
// Created by jjasmine on 3/7/24.
//

#pragma once
#include <algorithm>
#include <cassert>
#include <fmt/format.h>
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
  TokFunc,   // fn
  TokArrow,  // ->
  TokLet,    // let
  TokExtern, // extern
  // TokID
  TokID, // Representing an identifier

  // TokNum
  TokNum,   // Representing a number
  TokTrue,  // Representing a boolean true
  TokFalse, // Representing a boolean false
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
  size_t line_start, line_end;
  size_t col_start, col_end;
  size_t source_start; // True location in original source code string
  size_t source_end;

  // Default constructor
  Location()
      : line_start(0), line_end(0), col_start(0), col_end(0), source_start(0),
        source_end(0) {}

  Location(size_t line_start, size_t line_end, size_t col_start, size_t col_end,
           size_t source_start, size_t source_end)
      : line_start(line_start), line_end(line_end), col_start(col_start),
        col_end(col_end), source_start(source_start), source_end(source_end) {}
  // Advance column position
  inline void advance() {
    col_end++;
    source_end++;
  }

  // Move column position backwards
  inline void devance() {
    if (col_end == 0)
      col_end--;
    source_end--;
  }

  // Handle newline
  inline void newLine() {
    line_end++;
    col_end = 0;
  }

  // Combine two locations (union of spans)
  Location operator|(const Location &other) const {
    Location result;
    result.line_start = std::min(line_start, other.line_start);
    result.line_end = std::max(line_end, other.line_end);
    result.col_start =
        (line_start < other.line_start) ? col_start : other.col_start;
    result.col_end = (line_end > other.line_end) ? col_end : other.col_end;
    result.source_start = std::min(source_start, other.source_start);
    result.source_end = std::max(source_end, other.source_end);
    return result;
  }

  // Stream output operator
  friend std::ostream &operator<<(std::ostream &out, const Location &loc) {
    out << loc.line_start << ":" << loc.col_start;
    out << "-" << loc.line_end << ":" << loc.col_end;

    return out;
  }

  std::string to_string() {
    return fmt::format("{}:{}-{}:{}", line_start, col_start, line_end, col_end);
  }

  // Equality operator
  bool operator==(const Location &other) const {
    return source_start == other.source_start &&
           source_end == other.source_end && line_start == other.line_start &&
           line_end == other.line_end && col_start == other.col_start &&
           col_end == other.col_end;
  }

private:
  // String conversion operator
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
      : type(type), lexeme(std::move(lexeme)), location(location) {
    this->location.source_start = this->location.source_end - lexeme.size();
  };
};

//! A helper class for Lexer to simplify the process of getting a token.

//!
//!
class TokenStream {
  std::vector<std::shared_ptr<Token>> TokStream;
  size_t current_index;
  bool error;

public:
  std::vector<std::shared_ptr<Token>> ErrStream;

  TokenStream() : TokStream(), current_index(0), error(false) {}

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
      current_index = TokStream.size() - 1;
      return TokStream.back();
    }
    while (!isEnd()) {
      if (TokStream[current_index]->type == tokType)
        return TokStream[current_index];
      else
        current_index++;
    }

    return TokStream.back();
  }

  bool isEnd() { return current_index >= (TokStream.size() - 1); }
  std::shared_ptr<Token> peek() { return TokStream[current_index]; };
  std::shared_ptr<Token> consume() {
    auto token = peek();
    current_index = std::min(TokStream.size() - 1, current_index + 1);
    return token;
  }

  Location currentLocation() {
    if (!TokStream.empty()) {
      return TokStream[current_index]->location;
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
