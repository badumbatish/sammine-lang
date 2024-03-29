//
// Created by jjasmine on 3/8/24.
//

#include "Lexer.h"
#include "functional"
namespace sammine_lang {

using LexFunction = std::function<size_t(Lexer *, size_t, const std::string &)>;
std::shared_ptr<Token> Lexer::peek() { return tokStream->peek(); }

std::shared_ptr<Token> Lexer::consume() { return tokStream->consume(); }

Lexer::Lexer(const std::string &input) : Lexer() {
  size_t i = 0;
  size_t i_0 = 0;

  static std::vector<LexFunction> MatchFunctions = {
      &Lexer::handleID,
      &Lexer::handleNumber,
      &Lexer::handleOperators,
      &Lexer::handleUtility,
  };

  while (i < input.length()) {

    i = handleSpaces(i, input);

    for (auto fn : MatchFunctions) {
      i_0 = i;
      i = fn(this, i, input);
      if (i != i_0)
        break;
    }

    if (i == i_0 && i < input.length()) {
      handleInvalid(i, input);
    }
  }
  tokStream->push_back({TokEOF, "end of file", location});
}

size_t Lexer::handleID(size_t i, const std::string &input) {
  auto startLocation = location;
  if (isalpha(input[i])) { // identifier: [a-zA-Z][a-zA-Z0-9]*
    std::string IdentifierStr;
    IdentifierStr = input[i];

    i = advance(i);
    while (isalnum(input[i])) {
      IdentifierStr += input[i];
      i = advance(i);
    }

    if (IdentifierStr == "fn")
      tokStream->push_back(Token(TokFunc, "fn", location));
    else if (IdentifierStr == "return")
      tokStream->push_back(Token(TokReturn, "return", location));
    else if (IdentifierStr == "if")
      tokStream->push_back(Token(TokIf, "if", location));
    else if (IdentifierStr == "else")
      tokStream->push_back(Token(TokElse, "else", location));
    else if (IdentifierStr == "let")
      tokStream->push_back(Token(TokLet, "let", location));
    else if (IdentifierStr == "main")
      tokStream->push_back(Token(TokMain, "main", location));
    else
      tokStream->push_back(Token(TokID, IdentifierStr, location));
  }

  return i;
}

size_t Lexer::handleNumber(size_t i, const std::string &input) {
  std::string NumStr = {};

  if (isdigit(input[i])) {
    do {
      NumStr += input[i];
      i = advance(i);
    } while (i < input.length() - 1 && isdigit(input[i]));

    if (input[i] == '.') {
      NumStr += input[i];
      i = advance(i);
      do {
        NumStr += input[i];
        i = advance(i);
      } while (i < input.length() - 1 && isdigit(input[i]));
    }

    tokStream->push_back(Token(TokNum, NumStr, location));

  } else if (input[i] == '.') {
    int i_0 = i;
    NumStr += input[i];
    i = advance(i);

    while (i < input.length() - 1 && isdigit(input[i])) {
      NumStr += input[i];
      i = advance(i);
    }

    if (i - 1 == i_0) {
      tokStream->push_back(Token(TokDot, ".", location));
    } else {
      tokStream->push_back(Token(TokNum, NumStr, location));
    }
  }
  return i;
}

size_t Lexer::handleSpaces(size_t i, const std::string &input) {
  while (isspace(input[i])) {
    if (input[i] == '\n') {
      location.newLine();
    }
    i = advance(i);
  }

  return i;
}

size_t Lexer::handleInvalid(size_t i, const std::string &input) {
  tokStream->push_back(Token(TokINVALID, input.substr(i, 1), location));
  i = advance(i);
  return i;
}

size_t Lexer::handleOperators(size_t i, const std::string &input) {
  size_t i_0 = 0;
  i_0 = i;

  static std::vector<LexFunction> MatchFunctions = {
      &Lexer::handleOperatorsADD,      &Lexer::handleOperatorsSUB,
      &Lexer::handleOperatorsMUL,      &Lexer::handleOperatorsDIV,
      &Lexer::handleOperatorsMOD,      &Lexer::handleOperatorsAND,
      &Lexer::handleOperatorsOR,       &Lexer::handleOperatorsXOR,
      &Lexer::handleOperatorsCompLeft, &Lexer::handleOperatorsCompRight,
      &Lexer::handleOperatorsEqual,    &Lexer::handleOperatorsNot,
  };

  for (auto fn : MatchFunctions) {
    i_0 = i;
    i = fn(this, i, input);
    if (i != i_0)
      break;
  }

  return i;
}

size_t Lexer::handleOperatorsADD(size_t i, const std::string &input) {
  if (input[i] == '+') {

    // If the next index (i+1) is outside of input, we should return ADD
    if (input.length() - 1 < i + 1 ||
        (input[i + 1] != '+' && input[i + 1] != '=')) {
      tokStream->push_back(Token(TokADD, "+", location));
      i = advance(i);
      return i;
    }

    i = advance(i);

    if (input[i] == '+') {
      tokStream->push_back(Token(TokAddIncr, "++", location));
      i = advance(i);
      return i;
    }

    if (input[i] == '=') {
      tokStream->push_back(Token(TokAddAssign, "+=", location));
      i = advance(i);
      return i;
    }

    i = devance(i);
  }
  return i;
}

size_t Lexer::handleOperatorsSUB(size_t i, const std::string &input) {
  if (input[i] == '-') {

    // If the next index (i+1) is outside of input, we should return ADD
    if (input.length() - 1 < i + 1 ||
        (input[i + 1] != '-' && input[i + 1] != '=' && input[i + 1] != '>')) {
      tokStream->push_back(Token(TokSUB, "-", location));
      i = advance(i);
      return i;
    }

    i = advance(i);

    if (input[i] == '-') {
      tokStream->push_back(Token(TokSubDecr, "--", location));
      i = advance(i);
      return i;
    }

    if (input[i] == '>') {
      tokStream->push_back(Token(TokArrow, "->", location));
      i = advance(i);
      return i;
    }

    if (input[i] == '=') {
      tokStream->push_back(Token(TokSubAssign, "-=", location));
      i = advance(i);
      return i;
    }

    i = devance(i);
  }
  return i;
}

size_t Lexer::handleOperatorsMUL(size_t i, const std::string &input) {
  if (input[i] == '*') {

    // If the next index (i+1) is outside of input, we should return ADD
    if (input.length() - 1 < i + 1 ||
        (input[i + 1] != '*' && input[i + 1] != '=')) {
      tokStream->push_back(Token(TokMUL, "*", location));
      i = advance(i);
      return i;
    }

    i = advance(i);

    if (input[i] == '*') {
      tokStream->push_back(Token(TokEXP, "**", location));
      i = advance(i);
      return i;
    }

    if (input[i] == '=') {
      tokStream->push_back(Token(TokMulAssign, "*=", location));
      i = advance(i);
      return i;
    }

    i = devance(i);
  }
  return i;
}

size_t Lexer::handleOperatorsDIV(size_t i, const std::string &input) {
  if (input[i] == '/') {

    // If the next index (i+1) is outside of input, we should return ADD
    if (input.length() - 1 < i + 1 ||
        (input[i + 1] != '^' && input[i + 1] != '_' && input[i + 1] != '=')) {
      tokStream->push_back(Token(TokDIV, "/", location));
      i = advance(i);
      return i;
    }

    i = advance(i);

    if (input[i] == '^') {
      tokStream->push_back(Token(TokCeilDiv, "/^", location));
      i = advance(i);
      return i;
    }

    if (input[i] == '_') {
      tokStream->push_back(Token(TokFloorDiv, "/_", location));
      i = advance(i);
      return i;
    }

    if (input[i] == '=') {
      tokStream->push_back(Token(TokDivAssign, "/=", location));
      i = advance(i);
      return i;
    }

    i = devance(i);
  }
  return i;
}

size_t Lexer::handleOperatorsMOD(size_t i, const std::string &input) {
  if (input[i] == '%') {
    tokStream->push_back(Token(TokMOD, "%", location));
    i = advance(i);
  }
  return i;
}

size_t Lexer::handleOperatorsAND(size_t i, const std::string &input) {
  if (input[i] == '&') {

    // If the next index (i+1) is outside of input, we should return ADD
    if (input.length() - 1 < i + 1 || (input[i + 1] != '&')) {
      tokStream->push_back(Token(TokAndLogical, "&", location));
      i = advance(i);
      return i;
    }

    i = advance(i);

    if (input[i] == '&') {
      tokStream->push_back(Token(TokAND, "&", location));
      i = advance(i);
      return i;
    }

    i = devance(i);
  }
  return i;
}

size_t Lexer::handleOperatorsOR(size_t i, const std::string &input) {
  if (input[i] == '|') {

    // If the next index (i+1) is outside of input, we should return ADD
    if (input.length() - 1 < i + 1 || (input[i + 1] != '|')) {
      tokStream->push_back(Token(TokORLogical, "|", location));
      i = advance(i);
      return i;
    }

    i = advance(i);

    if (input[i] == '|') {
      tokStream->push_back(Token(TokOR, "||", location));
      i = advance(i);
      return i;
    }

    i = devance(i);
  }
  return i;
}

size_t Lexer::handleOperatorsXOR(size_t i, const std::string &input) {
  if (input[i] == '^') {
    tokStream->push_back(Token(TokXOR, "^", location));
    i = advance(i);
  }
  return i;
}

size_t Lexer::handleOperatorsCompLeft(size_t i, const std::string &input) {
  if (input[i] == '<') {

    // If the next index (i+1) is outside of input, we should return ADD
    if (input.length() - 1 < i + 1 ||
        (input[i + 1] != '<' && input[i + 1] != '=')) {
      tokStream->push_back(Token(TokLESS, "<", location));
      i = advance(i);
      return i;
    }

    i = advance(i);

    if (input[i] == '<') {
      tokStream->push_back(Token(TokSHL, "<<", location));
      i = advance(i);
      return i;
    }

    if (input[i] == '=') {
      tokStream->push_back(Token(TokLessEqual, "<=", location));
      i = advance(i);
      return i;
    }

    i = devance(i);
  }
  return i;
}

size_t Lexer::handleOperatorsCompRight(size_t i, const std::string &input) {
  if (input[i] == '>') {

    // If the next index (i+1) is outside of input, we should return ADD
    if (input.length() - 1 < i + 1 ||
        (input[i + 1] != '>' && input[i + 1] != '=')) {
      tokStream->push_back(Token(TokGREATER, ">", location));
      i = advance(i);
      return i;
    }

    i = advance(i);

    if (input[i] == '>') {
      tokStream->push_back(Token(TokSHR, ">>", location));
      i = advance(i);
      return i;
    }

    if (input[i] == '=') {
      tokStream->push_back(Token(TokGreaterEqual, ">=", location));
      i = advance(i);
      return i;
    }

    i = devance(i);
  }
  return i;
}

size_t Lexer::handleOperatorsEqual(size_t i, const std::string &input) {
  if (input[i] == '=') {

    // If the next index (i+1) is outside of input, we should return ADD
    if (input.length() - 1 < i + 1 || (input[i + 1] != '=')) {
      tokStream->push_back(Token(TokASSIGN, "=", location));
      i = advance(i);
      return i;
    }

    i = advance(i);
    if (input[i] == '=') {
      tokStream->push_back(Token(TokEQUAL, "==", location));
      i = advance(i);
      return i;
    }

    i = devance(i);
  }
  return i;
}

size_t Lexer::handleOperatorsNot(size_t i, const std::string &input) {
  if (input[i] == '!') {
    tokStream->push_back(Token(TokNOT, "!", location));
    i = advance(i);
  }
  return i;
}

size_t Lexer::handleUtility(size_t i, const std::string &input) {
  size_t i_0 = 0;
  i_0 = i;

  static std::vector<LexFunction> MatchFunctions = {
      &Lexer::handleUtilityPAREN,   &Lexer::handleUtilityCURLY,
      &Lexer::handleUtilityCOMMENT, &Lexer::handleUtilityCOMMA,
      &Lexer::handleUtilityCOLON,   &Lexer::handleUtilitySemiColon,
  };

  for (auto fn : MatchFunctions) {
    i_0 = i;
    i = fn(this, i, input);
    if (i != i_0)
      break;
  }

  return i;
}

size_t Lexer::handleUtilityPAREN(size_t i, const std::string &input) {
  if (input[i] == '(') {
    tokStream->push_back(Token(TokLeftParen, "(", location));
    i = advance(i);
    return i;
  }
  if (input[i] == ')') {
    tokStream->push_back(Token(TokRightParen, ")", location));
    i = advance(i);
    return i;
  }
  return i;
}

size_t Lexer::handleUtilityCURLY(size_t i, const std::string &input) {
  if (input[i] == '{') {
    tokStream->push_back(Token(TokLeftCurly, "{", location));
    i = advance(i);
    return i;
  }
  if (input[i] == '}') {
    tokStream->push_back(Token(TokRightCurly, "}", location));
    i = advance(i);
    return i;
  }
  return i;
}

size_t Lexer::handleUtilityCOMMENT(size_t i, const std::string &input) {
  if (input[i] == '#') {
    while (i < input.length() && input[i] != '\n') {
      i = advance(i);
    }
  }
  return i;
}

size_t Lexer::handleUtilityCOMMA(size_t i, const std::string &input) {
  if (input[i] == ',') {
    tokStream->push_back(Token(TokComma, ",", location));
    i = advance(i);
  }
  return i;
}

size_t Lexer::handleUtilityCOLON(size_t i, const std::string &input) {
  if (input[i] == ':') {
    if (input.length() - 1 < i + 1 || (input[i + 1] != ':')) {
      tokStream->push_back(Token(TokColon, ":", location));
      i = advance(i);
      return i;
    }
    i = advance(i);

    if (input[i] == ':') {
      tokStream->push_back(Token(TokDoubleColon, "::", location));
      i = advance(i);
      return i;
    }

    i = devance(i);
  }
  return i;
}

size_t Lexer::handleUtilitySemiColon(size_t i, const std::string &input) {
  if (input[i] == ';') {
    tokStream->push_back(Token(TokSemiColon, ";", location));
    i = advance(i);
  }
  return i;
}

size_t Lexer::advance(size_t i) {
  i++;
  location.advance();
  return i;
}

size_t Lexer::devance(size_t i) {
  if (i > 0)
    i--;
  location.devance();
  return i;
}
} // namespace sammine_lang
