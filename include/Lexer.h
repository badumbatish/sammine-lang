//
// Created by jjasmine on 3/7/24.
//

#ifndef SAMMINE_LANG_LEXER_H
#define SAMMINE_LANG_LEXER_H
//! \file Lexer.h
//! \brief Houses declaration (and some small implementation) of TokenType, Token, TokenStream, Lexer

#include <memory>
#include <utility>
#include <vector>
#include "FileRAII.h"

namespace sammine_lang {
enum TokenType {
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

    TokAND, // &&
    TokAndLogical, // &
    TokOR,  // ||
    TokORLogical, // |
    TokXOR, // ^
    TokSHL, // <<
    TokSHR, // >>

    TokEQUAL,   // ==
    TokLESS,    // <
    TokGREATER, // >
    TokASSIGN,  // =
    TokNOT,     // !


    //TokEXP AND FloorDiv
    TokEXP,      // **
    TokFloorDiv, // /_
    TokCeilDiv,  // /^

    //TokPAREN
    TokLeftParen, // (
    TokRightParen, // )
    TokLeftCurly, // {
    TokRightCurly, // }

    // Comma and colons and all that
    TokComma, // ,
    TokDot, // .
    TokColon, // :
    TokDoubleColon, // ::

    //TokFunction
    TokFunc, // func

    // TokID
    TokID, // Representing an identifier

    // TokNum
    TokNum, // Representing a number
    // TokIf
    TokIf, // if
    TokElse, // else

    //TokCOMMENTS
    TokSingleComment, //
    TokEOF,
    TokINVALID,
};

//! A class representing a token for sammine-lang, includes TokenType, lexeme and position pair as its members.

//! .
//! .
class Token {
public:
    TokenType type;
    std::string lexeme;
    std::pair<int, int> position;

    Token(TokenType type, std::string lexeme) : type(type), lexeme(std::move(lexeme)) {
        position = {0,0};
    };
};

//! A helper class for Lexer to simplify the process of getting a token.

//!
//!
class TokenStream {
    std::vector<std::shared_ptr<Token>> TokStream;
    size_t i;
public:
    TokenStream() {
        i = 0;
        TokStream = {};
    }

    void push_back(const std::shared_ptr<Token>& token) {
        TokStream.push_back(token);
    }

    void push_back(const Token& token) {
        TokStream.push_back(std::make_shared<Token>(token));
    }

    std::shared_ptr<Token> peek() {
        return TokStream[i];
    };
    std::shared_ptr<Token> consume() {
        auto token = peek();
        i = std::min(TokStream.size() - 1, i + 1);
        return token;
    }
};
//! A lexer for sammine-lang

//! Not sure what to put in here
//! Not sure what to put in here
class Lexer {
private:
    int column, row;

    TokenStream TokStream;

    Lexer() {
        column = 0;
        row = 0;
        TokStream = {};
    }

    size_t handleNumber(size_t i, const std::string& input);
    size_t handleSpaces(size_t i, const std::string& input);
    size_t handleID(size_t i, const std::string& input);
    size_t handleInvalid(size_t i, const std::string& input);
    size_t handleOperators(size_t i, const std::string& input);
    size_t handleOperatorsADD(size_t i, const std::string& input);
    size_t handleOperatorsSUB(size_t i, const std::string& input);
    size_t handleOperatorsMUL(size_t i, const std::string& input);
    size_t handleOperatorsDIV(size_t i, const std::string& input);
    size_t handleOperatorsMOD(size_t i, const std::string& input);
    size_t handleOperatorsAND(size_t i, const std::string& input);
    size_t handleOperatorsOR(size_t i, const std::string& input);
    size_t handleOperatorsXOR(size_t i, const std::string& input);
    size_t handleOperatorsSHIFT(size_t i, const std::string& input);
    size_t handleOperatorsCOMP(size_t i, const std::string& input);


    size_t handleUtility(size_t i, const std::string& input);
    size_t handleUtilityPAREN(size_t i, const std::string& input);
    size_t handleUtilityCURLY(size_t i, const std::string& input);
    size_t handleUtilityCOMMENT(size_t i, const std::string& input);
    size_t handleUtilityCOMMA(size_t i, const std::string& input);
    size_t handleUtilityDOT(size_t i, const std::string& input);
    size_t handleUtilityCOLON(size_t i, const std::string& input);


public:
    explicit Lexer(const std::string& input);
    explicit Lexer(FileRAII file) : Lexer(file.readFileToString()) {}

    std::shared_ptr<Token> peek();
    std::shared_ptr<Token> consume();

};
}
#endif //SAMMINE_LANG_LEXER_H
