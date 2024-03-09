//
// Created by jjasmine on 3/8/24.
//

#include "Lexer.h"
#include "functional"
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

    static std::vector<std::function<size_t (Lexer*, size_t, const std::string&)>> MatchFunctions = 
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
    TokStream.push_back({TokEOF, ""});
}

    size_t Lexer::handleID(size_t i, const std::string &input) {
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

    size_t Lexer::handleNumber(size_t i, const std::string &input) {
        std::string NumStr = {};
        if (isdigit(input[i])) {
            do {
                NumStr += input[i++];
            } while (i < input.length() -1 && isdigit(input[i]));

            if (input[i] == '.') {
                i++;
                do {
                    NumStr += input[i++];
                } while (i < input.length() -1 && isdigit(input[i]));
            }

            TokStream.push_back(Token(TokNum, NumStr));

        } 
        else if (input[i] == '.') {
            int i_0 = i;
            i++;

            while (i < input.length() -1 && isdigit(input[i])) {
                NumStr += input[i++];
            }

            if (i-1 == i_0) {
                TokStream.push_back(Token(TokDot, ""));
            } else {
                TokStream.push_back(Token(TokNum, NumStr));
            }
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

    size_t Lexer::handleOperators(size_t i, const std::string &input) {
        size_t i_0 = 0;
        i_0 = i;

        static std::vector<std::function<size_t (Lexer*, size_t, const std::string&)>> MatchFunctions = 
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
                TokStream.push_back(Token(TokADD, ""));
                i++;
                return i;
            }

            i++;

            if (input[i] == '+') {
                TokStream.push_back(Token(TokAddIncr, ""));
                i++;
                return i;
            } 

            if (input[i] == '=') {
                TokStream.push_back(Token(TokAddAssign, ""));
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
                TokStream.push_back(Token(TokSUB, ""));
                i++;
                return i;
            }

            i++;

            if (input[i] == '-') {
                TokStream.push_back(Token(TokSubDecr, ""));
                i++;
                return i;
            } 

            if (input[i] == '=') {
                TokStream.push_back(Token(TokSubAssign, ""));
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
                TokStream.push_back(Token(TokMUL, ""));
                i++;
                return i;
            }

            i++;

            if (input[i] == '*') {
                TokStream.push_back(Token(TokEXP, ""));
                i++;
                return i;
            } 

            if (input[i] == '=') {
                TokStream.push_back(Token(TokMulAssign, ""));
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
                TokStream.push_back(Token(TokDIV, ""));
                i++;
                return i;
            }

            i++;

            if (input[i] == '^') {
                TokStream.push_back(Token(TokCeilDiv, ""));
                i++;
                return i;
            } 

            if (input[i] == '_') {
                TokStream.push_back(Token(TokFloorDiv, ""));
                i++;
                return i;
            } 

            if (input[i] == '=') {
                TokStream.push_back(Token(TokDivAssign, ""));
                i++;
                return i;
            } 

            i--;
        }
        return i;
    }

    size_t Lexer::handleOperatorsMOD(size_t i, const std::string &input) {
        if (input[i] == '%') {
            TokStream.push_back(Token(TokMOD, ""));
            i++;
        }
        return i;
    }

    size_t Lexer::handleOperatorsAND(size_t i, const std::string &input) {
        if (input[i] == '&') {

            // If the next index (i+1) is outside of input, we should return ADD
            if (input.length() - 1 < i + 1 || (input[i+1] != '&')) {
                TokStream.push_back(Token(TokAndLogical, ""));
                i++;
                return i;
            }

            i++;

            if (input[i] == '&') {
                TokStream.push_back(Token(TokAND, ""));
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
                TokStream.push_back(Token(TokORLogical, ""));
                i++;
                return i;
            }

            i++;

            if (input[i] == '|') {
                TokStream.push_back(Token(TokOR, ""));
                i++;
                return i;
            } 
            
            i--;
        }
        return i;    
    }

    size_t Lexer::handleOperatorsXOR(size_t i, const std::string &input) {
        if (input[i] == '^') {
            TokStream.push_back(Token(TokXOR, ""));
            i++;
        }
        return i;
    }

    size_t Lexer::handleOperatorsCompLeft(size_t i, const std::string &input) {
        if (input[i] == '<') {

            // If the next index (i+1) is outside of input, we should return ADD
            if (input.length() - 1 < i + 1 || (input[i+1] != '<' && input[i+1] != '=')) {
                TokStream.push_back(Token(TokLESS, ""));
                i++;
                return i;
            }

            i++;

            if (input[i] == '<') {
                TokStream.push_back(Token(TokSHL, ""));
                i++;
                return i;
            } 

            if (input[i] == '=') {
                TokStream.push_back(Token(TokLessEqual, ""));
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
                TokStream.push_back(Token(TokGREATER, ""));
                i++;
                return i;
            }

            i++;

            if (input[i] == '>') {
                TokStream.push_back(Token(TokSHR, ""));
                i++;
                return i;
            } 

            if (input[i] == '=') {
                TokStream.push_back(Token(TokGreaterEqual, ""));
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
                TokStream.push_back(Token(TokASSIGN, ""));
                i++;
                return i;
            }

            i++;

            if (input[i] == '=') {
                TokStream.push_back(Token(TokEQUAL, ""));
                i++;
                return i;
            } 

            i--;
        }
        return i;
    }

    size_t Lexer::handleOperatorsNot(size_t i, const std::string &input) {
        if (input[i] == '!') {
            TokStream.push_back(Token(TokNOT, ""));
            i++;
        }
        return i;
    }

    size_t Lexer::handleUtility(size_t i, const std::string &input) {
        size_t i_0 = 0;
        i_0 = i;

        static std::vector<std::function<size_t (Lexer*, size_t, const std::string&)>> MatchFunctions = 
            {   
                &Lexer::handleUtilityPAREN,
                &Lexer::handleUtilityCURLY,
                &Lexer::handleUtilityCOMMENT,
                &Lexer::handleUtilityCOMMA,
                &Lexer::handleUtilityCOLON
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
            TokStream.push_back(Token(TokLeftParen, ""));
            i++;
            return i;
        }
        if (input[i] == ')') {
            TokStream.push_back(Token(TokRightParen, ""));
            i++;
            return i;
        }
        return i;
    }

    size_t Lexer::handleUtilityCURLY(size_t i, const std::string &input) {
        if (input[i] == '{') {
            TokStream.push_back(Token(TokLeftCurly, ""));
            i++;
            return i;
        }
        if (input[i] == '}') {
            TokStream.push_back(Token(TokRightCurly, ""));
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
            TokStream.push_back(Token(TokComma, ""));
            i++;
        }
        return i;
    }

    size_t Lexer::handleUtilityCOLON(size_t i, const std::string &input) {
        if (input[i] == ':') {
            if (input.length() - 1 < i + 1 || (input[i+1] != ':')) {
                TokStream.push_back(Token(TokColon, ""));
                i++;
                return i;
            }
            i++;

            if (input[i] == ':') {
                TokStream.push_back(Token(TokDoubleColon, ""));
                i++;
                return i;
            } 

            i--;
        }
        return i;        
    }
} // namespace sammine_lang
