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



Lexer::Lexer(const std::string& input) : Lexer() {
    int i = 0;

    while (i < input.length()) {
        while(isspace(input[i])) {
            i++;
        }

        if (isalpha(input[i])) { // identifier: [a-zA-Z][a-zA-Z0-9]*
            std::string IdentifierStr;
            IdentifierStr = input[i];

            while (isalnum(input[++i]))
                IdentifierStr += input[i];

            if (IdentifierStr == "func")
                TokStream.push_back(std::make_shared<Token>(Token(TokFunc, "")));
            else if (IdentifierStr == "if")
                TokStream.push_back(std::make_shared<Token>(Token(TokIf, "")));
            else if (IdentifierStr == "else")
                TokStream.push_back(std::make_shared<Token>(Token(TokElse, "")));
            else
                TokStream.push_back(std::make_shared<Token>(Token(TokID, IdentifierStr)));

            continue;
        }

        if (isdigit(input[i]) || input[i] == '.') { // Number: [0-9.]+
            std::string NumStr;
            do {
                NumStr += input[i++];
            } while (isdigit(input[i]) || input[i] == '.');

            TokStream.push_back(std::make_shared<Token>(Token(TokID, NumStr)));
            continue;
        }

    }

}

}