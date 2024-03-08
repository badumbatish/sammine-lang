//
// Created by jjasmine on 3/7/24.
//

#ifndef SAMMINE_LANG_LEXER_H
#define SAMMINE_LANG_LEXER_H

#include <memory>
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

    // TokIf
    TokIf, // if
    TokElse, // else

    //TokCOMMENTS
    TokSingleComment, //
    TokEOF,
    TokINVALID,
    DISCARD, // A Discard signal
};


struct Token {
    TokenType type;
    std::string lexeme;
    std::pair<int, int> position;
};

//! A lexer for sammine-lang

//! Not sure what to put in here
//! Not sure what to put in here
class Lexer {
private:
    int column, row;
    bool hasError;
    std::vector<std::shared_ptr<Token>> TokStream;

    std::shared_ptr<Token> lex(const std::string& str);
    Lexer() {
        column = 0;
        row = 0;
        hasError = false;
        TokStream = {};
    }
public:
    explicit Lexer(const std::string& input);
    explicit Lexer(FileRAII file);

    [[nodiscard]] bool hasErrors() const;

    std::shared_ptr<Token> peek();
    std::shared_ptr<Token> peek_n();
    std::shared_ptr<Token> expect(TokenType tok);
    void consume();

};
}
#endif //SAMMINE_LANG_LEXER_H
