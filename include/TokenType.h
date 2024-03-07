//
// Created by jjasmine on 3/7/24.
//

#ifndef SAMMINE_LANG_TOKENTYPE_H
#define SAMMINE_LANG_TOKENTYPE_H

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

    //TokFunction
    TokFunc, // func

    // TokIf
    TokIf, // if
    TokElse, // else

    //TokCOMMENTS
   TokSingleComment, //
   TokEOF,
   TokINVALID,


};
#endif //SAMMINE_LANG_TOKENTYPE_H
