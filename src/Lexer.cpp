//
// Created by jjasmine on 3/8/24.
//

#include "Lexer.h"
#include "functional"
namespace sammine_lang {

    using LexFunction = std::function<size_t (Lexer*, size_t, const std::string&)>;
std::shared_ptr<Token> Lexer::peek() {
    return tokStream->peek();
}

std::shared_ptr<Token> Lexer::consume() {
    return tokStream->consume();
}



Lexer::Lexer(const std::string& input) : Lexer() {
    size_t i = 0;
    size_t i_0 = 0;

    static std::vector<LexFunction> MatchFunctions =
        {   &Lexer::handleID,
            &Lexer::handleNumber,
            &Lexer::handleOperators,
            &Lexer::handleUtility,
            &Lexer::handleInvalid,
    };

    while (i < input.length()) {

        i = handleSpaces(i, input);

        for(auto fn : MatchFunctions) {
            i_0 = i;
            i = fn(this, i, input);
            if (i != i_0) break;
        }
        
    }
    tokStream->push_back({TokEOF, "", location});
}

    size_t Lexer::handleID(size_t i, const std::string &input) {
        if (isalpha(input[i])) { // identifier: [a-zA-Z][a-zA-Z0-9]*
            std::string IdentifierStr;
            IdentifierStr = input[i];

            while (isalnum(input[++i]))
                IdentifierStr += input[i];

            if (IdentifierStr == "func")
                tokStream->push_back(Token(TokFunc, "", location));
            else if (IdentifierStr == "if")
                tokStream->push_back(Token(TokIf, "", location));
            else if (IdentifierStr == "else")
                tokStream->push_back(Token(TokElse, "", location));
            else if (IdentifierStr == "let")
                tokStream->push_back(Token(TokLet, "", location));
            else
                tokStream->push_back(Token(TokID, IdentifierStr, location));
        }

        return i;
    }

    size_t Lexer::handleNumber(size_t i, const std::string &input) {
        std::string NumStr = {};
        if (isdigit(input[i])) {
            do {
                NumStr += input[i++];
            } while (i < input.length() -1 && isdigit(input[i]));

            if (input[i] == '.') {
                NumStr += input[i];
                i++;
                do {
                    NumStr += input[i++];
                } while (i < input.length() -1 && isdigit(input[i]));
            }

            tokStream->push_back(Token(TokNum, NumStr, location));

        } 
        else if (input[i] == '.') {
            int i_0 = i;
            NumStr += input[i];
            i++;

            while (i < input.length() -1 && isdigit(input[i])) {
                NumStr += input[i++];
            }

            if (i-1 == i_0) {
                tokStream->push_back(Token(TokDot, "", location));
            } else {
                tokStream->push_back(Token(TokNum, NumStr, location));
            }
        }
        return i;
    }

    size_t Lexer::handleSpaces(size_t i, const std::string &input) {
        while(isspace(input[i])) {
            if (input[i] == '\n') {
                location.newLine();
            }
            i++;
        }

        return i;
    }

    size_t Lexer::handleInvalid(size_t i, const std::string &input) {
        tokStream->push_back(Token(TokINVALID, input.substr(i, 1), location));
        i++;

        return i;
    }

    size_t Lexer::handleOperators(size_t i, const std::string &input) {
        size_t i_0 = 0;
        i_0 = i;

        static std::vector<LexFunction> MatchFunctions =
            {   
                &Lexer::handleOperatorsADD,
                &Lexer::handleOperatorsSUB,
                &Lexer::handleOperatorsMUL,
                &Lexer::handleOperatorsDIV,
                &Lexer::handleOperatorsMOD,
                &Lexer::handleOperatorsAND,
                &Lexer::handleOperatorsOR,
                &Lexer::handleOperatorsXOR,
                &Lexer::handleOperatorsCompLeft,
                &Lexer::handleOperatorsCompRight,
                &Lexer::handleOperatorsEqual,    
                &Lexer::handleOperatorsNot,
        };

        for(auto fn : MatchFunctions) {
            i_0 = i;
            i = fn(this, i, input);
            if (i != i_0) break;
        }

        return i;
    }

    size_t Lexer::handleOperatorsADD(size_t i, const std::string &input) {
        if (input[i] == '+') {

            // If the next index (i+1) is outside of input, we should return ADD
            if (input.length() - 1 < i + 1 || (input[i+1] != '+' && input[i+1] != '=')) {
                tokStream->push_back(Token(TokADD, "", location));
                i++;
                return i;
            }

            i++;

            if (input[i] == '+') {
                tokStream->push_back(Token(TokAddIncr, "", location));
                i++;
                return i;
            } 

            if (input[i] == '=') {
                tokStream->push_back(Token(TokAddAssign, "", location));
                i++;
                return i;
            } 
            
            i--;
        }
        return i;
    }

    size_t Lexer::handleOperatorsSUB(size_t i, const std::string &input) {
        if (input[i] == '-') {

            // If the next index (i+1) is outside of input, we should return ADD
            if (input.length() - 1 < i + 1 || (input[i+1] != '-' && input[i+1] != '=')) {
                tokStream->push_back(Token(TokSUB, "", location));
                i++;
                return i;
            }

            i++;

            if (input[i] == '-') {
                tokStream->push_back(Token(TokSubDecr, "", location));
                i++;
                return i;
            } 

            if (input[i] == '=') {
                tokStream->push_back(Token(TokSubAssign, "", location));
                i++;
                return i;
            } 

            i--;
        }
        return i;
    }

    size_t Lexer::handleOperatorsMUL(size_t i, const std::string &input) {
        if (input[i] == '*') {

            // If the next index (i+1) is outside of input, we should return ADD
            if (input.length() - 1 < i + 1 || (input[i+1] != '*' && input[i+1] != '=')) {
                tokStream->push_back(Token(TokMUL, "", location));
                i++;
                return i;
            }

            i++;

            if (input[i] == '*') {
                tokStream->push_back(Token(TokEXP, "", location));
                i++;
                return i;
            } 

            if (input[i] == '=') {
                tokStream->push_back(Token(TokMulAssign, "", location));
                i++;
                return i;
            } 

            i--;
        }
        return i;
    }

    size_t Lexer::handleOperatorsDIV(size_t i, const std::string &input) {
        if (input[i] == '/') {

            // If the next index (i+1) is outside of input, we should return ADD
            if (input.length() - 1 < i + 1 || (input[i+1] != '^' && input[i+1] != '_' && input[i+1] != '=')) {
                tokStream->push_back(Token(TokDIV, "", location));
                i++;
                return i;
            }

            i++;

            if (input[i] == '^') {
                tokStream->push_back(Token(TokCeilDiv, "", location));
                i++;
                return i;
            } 

            if (input[i] == '_') {
                tokStream->push_back(Token(TokFloorDiv, "", location));
                i++;
                return i;
            } 

            if (input[i] == '=') {
                tokStream->push_back(Token(TokDivAssign, "", location));
                i++;
                return i;
            } 

            i--;
        }
        return i;
    }

    size_t Lexer::handleOperatorsMOD(size_t i, const std::string &input) {
        if (input[i] == '%') {
            tokStream->push_back(Token(TokMOD, "", location));
            i++;
        }
        return i;
    }

    size_t Lexer::handleOperatorsAND(size_t i, const std::string &input) {
        if (input[i] == '&') {

            // If the next index (i+1) is outside of input, we should return ADD
            if (input.length() - 1 < i + 1 || (input[i+1] != '&')) {
                tokStream->push_back(Token(TokAndLogical, "", location));
                i++;
                return i;
            }

            i++;

            if (input[i] == '&') {
                tokStream->push_back(Token(TokAND, "", location));
                i++;
                return i;
            } 

            i--;
        }
        return i;
    }

    size_t Lexer::handleOperatorsOR(size_t i, const std::string &input) {
        if (input[i] == '|') {

            // If the next index (i+1) is outside of input, we should return ADD
            if (input.length() - 1 < i + 1 || (input[i+1] != '|')) {
                tokStream->push_back(Token(TokORLogical, "", location));
                i++;
                return i;
            }

            i++;

            if (input[i] == '|') {
                tokStream->push_back(Token(TokOR, "", location));
                i++;
                return i;
            } 
            
            i--;
        }
        return i;    
    }

    size_t Lexer::handleOperatorsXOR(size_t i, const std::string &input) {
        if (input[i] == '^') {
            tokStream->push_back(Token(TokXOR, "", location));
            i++;
        }
        return i;
    }

    size_t Lexer::handleOperatorsCompLeft(size_t i, const std::string &input) {
        if (input[i] == '<') {

            // If the next index (i+1) is outside of input, we should return ADD
            if (input.length() - 1 < i + 1 || (input[i+1] != '<' && input[i+1] != '=')) {
                tokStream->push_back(Token(TokLESS, "", location));
                i++;
                return i;
            }

            i++;

            if (input[i] == '<') {
                tokStream->push_back(Token(TokSHL, "", location));
                i++;
                return i;
            } 

            if (input[i] == '=') {
                tokStream->push_back(Token(TokLessEqual, "", location));
                i++;
                return i;
            } 

            i--;
        }
        return i;
    }

    size_t Lexer::handleOperatorsCompRight(size_t i, const std::string &input) {
        if (input[i] == '>') {

            // If the next index (i+1) is outside of input, we should return ADD
            if (input.length() - 1 < i + 1 || (input[i+1] != '>' && input[i+1] != '=')) {
                tokStream->push_back(Token(TokGREATER, "", location));
                i++;
                return i;
            }

            i++;

            if (input[i] == '>') {
                tokStream->push_back(Token(TokSHR, "", location));
                i++;
                return i;
            } 

            if (input[i] == '=') {
                tokStream->push_back(Token(TokGreaterEqual, "", location));
                i++;
                return i;
            } 

            i--;
        }
        return i;
    }

    size_t Lexer::handleOperatorsEqual(size_t i, const std::string &input) {
        if (input[i] == '=') {

            // If the next index (i+1) is outside of input, we should return ADD
            if (input.length() - 1 < i + 1 || (input[i+1] != '=')) {
                tokStream->push_back(Token(TokASSIGN, "", location));
                i++;
                return i;
            }

            i++;

            if (input[i] == '=') {
                tokStream->push_back(Token(TokEQUAL, "", location));
                i++;
                return i;
            } 

            i--;
        }
        return i;
    }

    size_t Lexer::handleOperatorsNot(size_t i, const std::string &input) {
        if (input[i] == '!') {
            tokStream->push_back(Token(TokNOT, "", location));
            i++;
        }
        return i;
    }

    size_t Lexer::handleUtility(size_t i, const std::string &input) {
        size_t i_0 = 0;
        i_0 = i;

        static std::vector<LexFunction> MatchFunctions =
            {   
                &Lexer::handleUtilityPAREN,
                &Lexer::handleUtilityCURLY,
                &Lexer::handleUtilityCOMMENT,
                &Lexer::handleUtilityCOMMA,
                &Lexer::handleUtilityCOLON,
                &Lexer::handleUtilitySemiColon,
            };

        for(auto fn : MatchFunctions) {
            i_0 = i;
            i = fn(this, i, input);
            if (i != i_0) break;
        }

        return i;
    }

    size_t Lexer::handleUtilityPAREN(size_t i, const std::string &input) {
        if (input[i] == '(') {
            tokStream->push_back(Token(TokLeftParen, "", location));
            i++;
            return i;
        }
        if (input[i] == ')') {
            tokStream->push_back(Token(TokRightParen, "", location));
            i++;
            return i;
        }
        return i;
    }

    size_t Lexer::handleUtilityCURLY(size_t i, const std::string &input) {
        if (input[i] == '{') {
            tokStream->push_back(Token(TokLeftCurly, "", location));
            i++;
            return i;
        }
        if (input[i] == '}') {
            tokStream->push_back(Token(TokRightCurly, "", location));
            i++;
            return i;
        }
        return i;    
    }

    size_t Lexer::handleUtilityCOMMENT(size_t i, const std::string &input) {
      if (input[i] == '#') {
        while (i < input.length() && input[i] != '\n' ) {
            i++;
        }

      }
      return i;
    }

    size_t Lexer::handleUtilityCOMMA(size_t i, const std::string &input) {
        if (input[i] == ',') {
            tokStream->push_back(Token(TokComma, "", location));
            i++;
        }
        return i;
    }

    size_t Lexer::handleUtilityCOLON(size_t i, const std::string &input) {
        if (input[i] == ':') {
            if (input.length() - 1 < i + 1 || (input[i+1] != ':')) {
                tokStream->push_back(Token(TokColon, "", location));
                i++;
                return i;
            }
            i++;

            if (input[i] == ':') {
                tokStream->push_back(Token(TokDoubleColon, "", location));
                i++;
                return i;
            } 

            i--;
        }
        return i;        
    }

    size_t Lexer::handleUtilitySemiColon(size_t i, const std::string &input) {
        if (input[i] == ';') {
            tokStream->push_back(Token(TokSemiColon, "", location));
            i++;
        }
        return i;
    }
} // namespace sammine_lang
