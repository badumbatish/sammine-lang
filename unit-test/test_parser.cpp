//
// Created by jjasmine on 3/16/24.
//

//! \file test_parser.cpp
//! \brief The unit-test file for all things related to a parser.
#include "FileRAII.h"
#include "Lexer.h"
#include "Parser.h"
#include <catch2/catch_test_macros.hpp>

using namespace sammine_lang;

TEST_CASE("Empty program parsing", "[Parser]") {
  auto lexer = sammine_lang::Lexer("");
  auto tokStreamPtr = lexer.getTokenStream();
  auto pg = sammine_lang::Parser(tokStreamPtr);

  auto programAST = pg.Parse();

  REQUIRE(programAST->DefinitionVec.empty() == true);
}

TEST_CASE("Variable definition parsing", "[Parser]") {

  SECTION("Variable definition with number") {
    auto lexer = sammine_lang::Lexer("let b : blablabla = 0;");
    auto tokStreamPtr = lexer.getTokenStream();
    auto pg = sammine_lang::Parser(tokStreamPtr);

    auto programAST = pg.Parse();

    REQUIRE(programAST->DefinitionVec.size() == 1);

    auto varDef = static_cast<sammine_lang::AST::VarDefAST *>(
        programAST->DefinitionVec.front().get());
    REQUIRE(varDef->TypedVar->name == "b");
    REQUIRE(varDef->TypedVar->type == "blablabla");
  }

  SECTION("Variable definition with binary expression") {
    auto lexer = sammine_lang::Lexer("let b : blablabla = 1+2;");
    auto tokStreamPtr = lexer.getTokenStream();
    auto pg = sammine_lang::Parser(tokStreamPtr);

    auto programAST = pg.Parse();

    REQUIRE(!pg.hasErrors());
    REQUIRE(programAST->DefinitionVec.size() == 1);

    auto varDef = static_cast<sammine_lang::AST::VarDefAST *>(
        programAST->DefinitionVec.front().get());
    REQUIRE(varDef->TypedVar->name == "b");
    REQUIRE(varDef->TypedVar->type == "blablabla");
  }

  SECTION("Variable definition with call expression") {
    auto lexer = sammine_lang::Lexer("let b : blablabla = hi();");
    auto tokStreamPtr = lexer.getTokenStream();
    auto pg = sammine_lang::Parser(tokStreamPtr);

    auto programAST = pg.Parse();

    REQUIRE(programAST->DefinitionVec.size() == 1);

    auto varDef = static_cast<sammine_lang::AST::VarDefAST *>(
        programAST->DefinitionVec.front().get());
    REQUIRE(varDef->TypedVar->name == "b");
    REQUIRE(varDef->TypedVar->type == "blablabla");

    auto expr =
        static_cast<sammine_lang::AST::CallExprAST *>(varDef->Expression.get());
    REQUIRE(expr->functionName == "hi");
    REQUIRE(expr->arguments->size() == 0);
  }

  // SECTION("Variable definition with string literal expression") {
  //     auto lexer = sammine_lang::Lexer("let b : blablabla = \"how about
  //     this\" "); auto tokStreamPtr = lexer.getTokenStream(); auto pg =
  //     sammine_lang::Parser(tokStreamPtr);

  //     auto programAST = pg.Parse();

  //     REQUIRE(programAST->DefinitionVec.size() == 1);

  //     auto varDef =
  //     static_cast<sammine_lang::AST::VarDefAST*>(programAST->DefinitionVec.front().get());
  //     REQUIRE(varDef->TypedVar->name == "b");
  //     REQUIRE(varDef->TypedVar->type == "blablabla");
  // }
}

TEST_CASE("Function declaration parsing", "[Parser]") {
  // func f(x:f64) -> f64 {
  //  return 0;
  //}

  SECTION("Single argument") {
    auto lex = Lexer("fn f(x:f64) -> f64 {\n return 0 ; \n }");
    REQUIRE(lex.getTokenStream()->hasErrors() == false);
    auto pg = Parser(lex.getTokenStream());

    auto programAST = pg.Parse();
    REQUIRE(programAST->DefinitionVec.size() == 1);

    auto func_def =
        std::unique_ptr<AST::FuncDefAST>(dynamic_cast<AST::FuncDefAST *>(
            programAST->DefinitionVec.front().release()));

    // Check if downcast is valid.
    REQUIRE(func_def != nullptr);

    REQUIRE(func_def->Prototype->returnType == "f64");
    REQUIRE(func_def->Prototype->functionName == "f");
    REQUIRE(func_def->Prototype->parameterVectors->size() == 1);
    REQUIRE(func_def->Block->Statements.empty());
    REQUIRE(func_def->Block->returnStmt != nullptr);
  }

  SECTION("Multiple argument") {
    auto lex = Lexer("fn f(x:f64, y : hi, z : hoe) -> f64 {\n return 1+2 ; \n }");
    REQUIRE(lex.getTokenStream()->hasErrors() == false);
    auto pg = Parser(lex.getTokenStream());

    auto programAST = pg.Parse();
    REQUIRE(programAST->DefinitionVec.size() == 1);

    auto func_def =
        std::unique_ptr<AST::FuncDefAST>(dynamic_cast<AST::FuncDefAST *>(
            programAST->DefinitionVec.front().release()));

    // Check if downcast is valid.
    REQUIRE(func_def != nullptr);

    REQUIRE(func_def->Prototype->returnType == "f64");
    REQUIRE(func_def->Prototype->functionName == "f");
    REQUIRE(func_def->Prototype->parameterVectors->size() == 3);
    REQUIRE(func_def->Block->Statements.empty());
    REQUIRE(func_def->Block->returnStmt != nullptr);
  }


}

TEST_CASE("FAILED TO PARSE", "[Parser]") {
  auto lex = Lexer("a a a a a");
  REQUIRE(lex.getTokenStream()->hasErrors() == false);
  auto pg = Parser(lex.getTokenStream());
  auto programAST = pg.Parse();
  REQUIRE(pg.hasErrors() == true);
}