//
// Created by jjasmine on 3/7/24.
//

#ifndef SAMMINE_LANG_LEXER_H
#define SAMMINE_LANG_LEXER_H

#include <memory>
#include <utility>
#include <vector>
#include <map>
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
    TokOR,  // ||
    TokXOR, // ^
    TokSHL, // <<
    TokSHR, // >>

    TokEQUAL,   // ==
    TokLESS,    // <
    TokGREATER, // >
    TokASSIGN,  // =
    TokNOT,     // !

    TokAndAssign,    // &=
    TokOrAssign,     // !=
    TokXorAssign,    // ^=
    TokShlAssign,    // <<=
    TokShrAssign,    // >>=
    TokAndNotAssign, // &^=

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
    TokColon, // .
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
    DISCARD, // A Discard signal
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

    void push_back(Token token) {
        TokStream.push_back(std::make_shared<Token>(std::move(token)));
    }

    std::shared_ptr<Token> peek() {
        return TokStream[i];
    };
    void consume() {
        i = std::min(TokStream.size() - 1, i + 1);
    }
};
//! A lexer for sammine-lang

//! Not sure what to put in here
//! Not sure what to put in here
class Lexer {
private:
    int column, row;
    bool hasError;

    TokenStream TokStream;

    Lexer() {
        column = 0;
        row = 0;
        hasError = false;
        TokStream = {};
    }
public:
    explicit Lexer(const std::string& input);
    explicit Lexer(FileRAII file) : Lexer(file.readFileToString()) {}

    [[nodiscard]] bool hasErrors() const;

    std::shared_ptr<Token> peek();
    void consume();

};
}
#endif //SAMMINE_LANG_LEXER_H
