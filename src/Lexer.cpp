//
// Created by jjasmine on 3/8/24.
//

#include "Lexer.h"

namespace sammine_lang {


std::shared_ptr <Token> Lexer::lex(const std::string& str) {

    return nullptr;
}


bool Lexer::hasErrors() const {
    return false;
}

std::shared_ptr<Token> Lexer::peek() {
    return {};
}

std::shared_ptr<Token> Lexer::peek_n() {
    return {};
}

std::shared_ptr<Token> Lexer::expect(TokenType tok) {
    return {};
}

void Lexer::consume() {

}

Lexer::Lexer(FileRAII file) : Lexer() {
    std::string sub_str = {};
    while (!file.isEOF()) {

    }
}

Lexer::Lexer(const std::string& input) : Lexer() {
    int i = 0;

    while (i < input.length()) {

    }

}

}