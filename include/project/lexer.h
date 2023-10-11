//
// Created by Jasmine Tang on 10/3/23.
//
#include <string>
#pragma once

class lexer {
public:
    static inline std::string IdentifierStr {}; // Filled in if tok_identifier
    static inline double NumVal {};             // Filled in if tok_number

public:
    static int gettok();

};

// The lexer returns tokens [0-255] if it is an unknown character, otherwise one
// of these for known things.
enum Token {
    tok_eof = -1,

    // commands
    tok_def = -2,
    tok_extern = -3,

    // primary
    tok_identifier = -4,
    tok_number = -5,
};

