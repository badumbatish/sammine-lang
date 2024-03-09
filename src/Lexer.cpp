//
// Created by jjasmine on 3/8/24.
//

#include "Lexer.h"

namespace sammine_lang {


std::shared_ptr<Token> Lexer::peek() {
    return TokStream.peek();
}

std::shared_ptr<Token> Lexer::consume() {
    return TokStream.consume();
}



Lexer::Lexer(const std::string& input) : Lexer() {
    size_t i = 0;
    size_t i_0 = 0;
    while (i < input.length()) {

        i = handleSpaces(i, input);


        i_0 = i;
        i = handleID(i, input);
        if (i != i_0) continue;

        i_0 = i;
        i = handleNumber(i, input);
        if (i != i_0) continue;


        i = handleInvalid(i, input);
    }

}

    size_t Lexer::handleNumber(size_t i, const std::string &input) {
        if (isalpha(input[i])) { // identifier: [a-zA-Z][a-zA-Z0-9]*
            std::string IdentifierStr;
            IdentifierStr = input[i];

            while (isalnum(input[++i]))
                IdentifierStr += input[i];

            if (IdentifierStr == "func")
                TokStream.push_back(Token(TokFunc, ""));
            else if (IdentifierStr == "if")
                TokStream.push_back(Token(TokIf, ""));
            else if (IdentifierStr == "else")
                TokStream.push_back(Token(TokElse, ""));
            else
                TokStream.push_back(Token(TokID, IdentifierStr));
        }

        return i;
    }

    size_t Lexer::handleID(size_t i, const std::string &input) {
        if (isdigit(input[i]) || input[i] == '.') { // Number: [0-9.]+
            std::string NumStr;
            do {
                NumStr += input[i++];
            } while (isdigit(input[i]) || input[i] == '.');

            TokStream.push_back(Token(TokNum, NumStr));
        }

        return i;
    }

    size_t Lexer::handleSpaces(size_t i, const std::string &input) {
        while(isspace(input[i])) {
            i++;
        }

        return i;
    }

    size_t Lexer::handleInvalid(size_t i, const std::string &input) {
        TokStream.push_back(Token(TokINVALID, input.substr(i, 1)));
        i++;

        return i;
    }

}