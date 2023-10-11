//
// Created by Jasmine Tang on 10/3/23.
//

#include "lexer.h"

int lexer::gettok() {
    static int LastChar = ' ';

    // Skip any whitespace.
    while (isspace(LastChar))
        LastChar = getchar();

    if (isalpha(LastChar)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
        lexer::IdentifierStr = LastChar;
        while (isalnum((LastChar = getchar())))
            lexer::IdentifierStr += LastChar;

        if (IdentifierStr == "def")
            return Token::tok_def;
        if (IdentifierStr == "extern")
            return Token::tok_extern;
        return Token::tok_identifier;
    }

    if (isdigit(LastChar) || LastChar == '.') {   // Number: [0-9.]+
        std::string NumStr;
        do {
            NumStr += LastChar;
            LastChar = getchar();
        } while (isdigit(LastChar) || LastChar == '.');

        lexer::NumVal = strtod(NumStr.c_str(), 0);
        return Token::tok_number;
    }

    if (LastChar == '#') {
        // Comment until end of line.
        do
            LastChar = getchar();
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF)
            return gettok();
    }

    // Check for end of file.  Don't eat the EOF.
    if (LastChar == EOF)
        return Token::tok_eof;

    // Otherwise, just return the character as its ascii value.
    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}
