//
// Created by jjasmine on 3/7/24.
//

#ifndef SAMMINE_LANG_TOKENTYPE_H
#define SAMMINE_LANG_TOKENTYPE_H
#include <string>
#include <utility>
///
/// Hi there this is a TokenType enum
///
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
};


struct Token {
    TokenType type;
    std::string lexeme;
    std::pair<int, int> position;
};
#endif //SAMMINE_LANG_TOKENTYPE_H
