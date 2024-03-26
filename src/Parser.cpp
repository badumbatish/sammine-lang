
#include <functional>
#include "Parser.h"


namespace sammine_lang {
    static std::map<TokenType, int> binopPrecedence = { {TokenType::TokLESS, 10},
                                                       {TokenType::TokADD, 20},
                                                       {TokenType::TokSUB, 20},
                                                       {TokenType::TokMUL, 40}};

    int GetTokPrecedence(TokenType tokType) {
      int TokPrec = binopPrecedence[tokType];
      if (TokPrec <= 0) return -1;
      return TokPrec;
    }

    auto Parser::Parse() -> std::unique_ptr<AST::ProgramAST> {
        return ParseProgram();
    }

    auto Parser::ParseProgram() -> std::unique_ptr<AST::ProgramAST> {
        auto programAST = std::make_unique<AST::ProgramAST>();
        while (!tokStream->isEnd()) {
            auto def = ParseDefinition();
            if (def != nullptr)  {
              programAST->DefinitionVec.push_back(std::move(def));
            }
        }

        return programAST;
    }

    auto Parser::ParseDefinition() -> std::unique_ptr<AST::DefinitionAST> {
        using ParseFunction = std::function<std::unique_ptr<AST::DefinitionAST> (Parser*)>;
        std::vector<std::pair<ParseFunction, bool>> ParseFunctions = {
                {&Parser::ParseFuncDef, false},
                {&Parser::ParseVarDef, false},
        };

        for (auto [fn, required] : ParseFunctions) {
            auto result = fn(this);
            if (result != nullptr) return result;
        }

        expect(TokenType::TokINVALID, true, TokEOF, "Failed to parse any meaningful definitions");
        return nullptr;
    }

    auto Parser::ParseFuncDef() -> std::unique_ptr<AST::DefinitionAST> {
        auto fn = expect(TokenType::TokFunc);
        if (fn == nullptr) return nullptr;

        auto prototype = ParsePrototype();
        if (prototype == nullptr) {
          expect(TokenType::TokINVALID, true, TokRightCurly, "Failed to parse a prototype of a function");
          return std::make_unique<AST::FuncDefAST>(nullptr, nullptr);
        }

        auto block = ParseBlock();
        if (!block) {
          expect(TokenType::TokINVALID, true, TokRightCurly,
                 "Failed to parse a block");
          return std::make_unique<AST::FuncDefAST>(std::move(prototype), nullptr);
        }

        return std::make_unique<AST::FuncDefAST>(std::move(prototype), std::move(block));
    }

    //! Parsing implementation for a variable decl/def

    //! Accepts a let, continue parsing inside and (enable error reporting if possible).
    //! If a `let` is not found then return a nullptr.
    auto Parser::ParseVarDef() -> std::unique_ptr<AST::DefinitionAST> {
        auto let = expect(TokenType::TokLet);
        if (let == nullptr) return nullptr;
        auto typedVar = ParseTypedVar();
        if (!typedVar) {
          expect(TokenType::TokINVALID, true, TokSemiColon, "Failed to parse typed variable");
          return std::make_unique<AST::VarDefAST>(nullptr, nullptr);
        }

        auto assign = expect(TokenType::TokASSIGN, true, TokSemiColon, "Failed to match assign token `=`");
        if (!assign) return std::make_unique<AST::VarDefAST>(nullptr, nullptr);

        auto expr = ParseExpr();
        if (!expr) {
          expect(TokenType::TokINVALID, true, TokSemiColon, "Failed to parse expression");
          return std::make_unique<AST::VarDefAST>(nullptr, nullptr);
        }

        auto semicolon = expect(TokenType::TokSemiColon, true, TokSemiColon, "Failed to match semicolon token `;`");

        auto varDef = std::make_unique<AST::VarDefAST>(std::move(typedVar), std::move(expr));

        return varDef;
    }

    auto Parser::ParseTypedVar() -> std::unique_ptr<AST::TypedVarAST> {
        auto name = expect(TokenType::TokID);
        if (!name) return nullptr;
        auto colon = expect(TokenType::TokColon);
        if (!colon) return nullptr;
        auto type = expect(TokenType::TokID);
        if (!type) return nullptr;

        auto typedVar = std::make_unique<AST::TypedVarAST>();

        typedVar->name = name->lexeme;
        typedVar->type = type->lexeme;
        return typedVar;
    }
    auto Parser::ParsePrimaryExpr() -> std::unique_ptr<AST::ExprAST> {
      using ParseFunction = std::function<std::unique_ptr<AST::ExprAST> (Parser*)>;
      std::vector<ParseFunction> ParseFunctions = {
          &Parser::ParseCallExpr,
          &Parser::ParseNumberExpr,
          &Parser::ParseVariableExpr,
      };

      for (auto fn : ParseFunctions) {
        auto result = fn(this);
        if (result != nullptr) return result;
      }

      return nullptr;
    }
    auto Parser::ParseExpr() -> std::unique_ptr<AST::ExprAST> {
        auto LHS = ParsePrimaryExpr();
        if (!LHS) return nullptr;

        return ParseBinaryExpr(0, std::move(LHS));
    }

    auto Parser::ParseBinaryExpr(int prededence,
                                 std::unique_ptr<AST::ExprAST> LHS)
        -> std::unique_ptr<AST::ExprAST> {
      while (true) {
        int TokPrec = GetTokPrecedence(tokStream->peek()->type);

        if (TokPrec < prededence) return LHS;

        auto binOpToken = tokStream->consume();

        auto RHS = ParsePrimaryExpr();
        if (!RHS) return nullptr;

        int NextPrec = GetTokPrecedence(tokStream->peek()->type);
        if (TokPrec < NextPrec) {
          RHS = ParseBinaryExpr(TokPrec + 1, std::move(RHS));
          if (!RHS) return nullptr;
        }

        LHS = std::make_unique<AST::BinaryExprAST>(binOpToken, std::move(LHS), std::move(RHS));
      }
    }

    auto Parser::ParseCallExpr() -> std::unique_ptr<AST::ExprAST> {
        auto id = expect(TokenType::TokID);
        if (id == nullptr) return nullptr;
        else {
          auto args = ParseArguments();

          return std::make_unique<AST::CallExprAST>(id->lexeme, std::move(args));
        }

        return nullptr;
    }

    auto Parser::ParseNumberExpr() -> std::unique_ptr<AST::ExprAST> {
        auto numberExpr = std::make_unique<AST::NumberExprAST>();

        auto numberToken = expect(TokenType::TokNum);

        if (numberToken == nullptr) {
          return nullptr;
        } else {
            numberExpr->number = numberToken->lexeme;
        }

        return numberExpr;
    }

    auto Parser::ParseVariableExpr() -> std::unique_ptr<AST::ExprAST> {
        return {};
    }

    auto Parser::ParsePrototype() -> std::unique_ptr<AST::PrototypeAST> {
        auto id = expect(TokID);
        if (!id) return nullptr;

        auto params = ParseParams();
        if (!params) return nullptr;
        auto arrow = expect(TokArrow);
        if (!arrow) return nullptr;

        auto returnType = expect(TokID);
        if (!returnType) return nullptr;

        return std::make_unique<AST::PrototypeAST>(id->lexeme, returnType->lexeme, std::move(params));
    }

    auto Parser::ParseBlock() -> std::unique_ptr<AST::BlockAST> {

      auto leftCurly = expect(TokLeftCurly);
      if (!leftCurly) return nullptr;
      // TODO : Cannot just parse a return stmt.
      // TODO : We need to also parse other statement as well
      auto returnStmt = ParseReturnStmt();
      auto rightCurly = expect(TokRightCurly);

      if (!rightCurly) return nullptr;

      auto blockAST = std::make_unique<AST::BlockAST>();


      blockAST->returnStmt = std::move(returnStmt);
      return blockAST;
    }

    auto Parser::ParseReturnStmt() -> std::unique_ptr<AST::ExprAST> {
        auto returnTok = expect(TokReturn);
        auto expr = ParseExpr();
        auto semiColon = expect(TokSemiColon);
        return expr;
    }
    auto Parser::ParseStmt() -> std::unique_ptr<AST::StmtAST> {
        return {};
    }

    auto Parser::ParseSimpleStmt() -> std::unique_ptr<AST::SimpleStmtAST> {
        return {};
    }

    auto Parser::ParseIfStmtStmt() -> std::unique_ptr<AST::IfStmtAST> {
        return {};
    }

    // Parsing of parameters in a function call, we use leftParen and rightParen as appropriate stopping point
    auto Parser::ParseParams() -> std::unique_ptr<std::vector<std::unique_ptr<AST::TypedVarAST>>> {
        auto leftParen = expect(TokLeftParen);
        if (leftParen == nullptr) return {};

        std::unique_ptr<std::vector<std::unique_ptr<AST::TypedVarAST>>> vec = std::make_unique<std::vector<std::unique_ptr<AST::TypedVarAST>>>();
        auto typeVar = ParseTypedVar();
        if (typeVar != nullptr) { vec->push_back(std::move(typeVar)); }
        while (true) {
            //auto typeVar = ParseTypedVar();
            auto comma = expect(TokComma);
            if (comma == nullptr) break;

            typeVar = ParseTypedVar();
            if (typeVar == nullptr) return {};
        }
        auto rightParen = expect(TokRightParen);
        if (rightParen == nullptr) return nullptr;

        return vec;
    }

    auto Parser::ParseArguments() -> std::unique_ptr<std::vector<std::unique_ptr<AST::ExprAST>>> {
      auto leftParen = expect(TokLeftParen);
      if (leftParen == nullptr) return {};

      std::unique_ptr<std::vector<std::unique_ptr<AST::ExprAST>>> vec = std::make_unique<std::vector<std::unique_ptr<AST::ExprAST>>>();

      auto expr = ParseExpr();
      if (expr != nullptr) { vec->push_back(std::move(expr)); }
      while (true) {
        //auto typeVar = ParseTypedVar();
        auto comma = expect(TokComma);
        if (comma == nullptr) break;

        expr = ParseExpr();
        if (expr == nullptr) return {};
      }
      auto rightParen = expect(TokRightParen);
      if (rightParen == nullptr) return nullptr;

      return vec;
    }

    auto Parser::expect(TokenType tokType, bool exhausts, TokenType until, const std::string& message) -> std::shared_ptr<Token> {
        auto currentToken = tokStream->peek();
        auto result = !tokStream->isEnd() && currentToken->type == tokType;
        if (result) {
            return tokStream->consume();
        } else {
            // TODO: Add error reporting after this point.
            if (exhausts) {
              auto it = TokenMap.find(tokType);
              auto tok =  it != TokenMap.end() ? it->second : "`invalid`";
              error_msgs.push_back(message +  " at " + tokStream->currentLocation().to_string());
              tokStream->exhaust_until(until);
              return nullptr;
            } else {
              return nullptr;
            }
        }
    }

    }

