//
// Created by jjasmine on 3/16/24.
//

//! \file test_parser.cpp
//! \brief The unit-test file for all things related to a parser.
#include <catch2/catch_test_macros.hpp>
#include "Parser.h"
#include "Lexer.h"

TEST_CASE("Empty program parsing", "[Parser]") {
    auto lexer = sammine_lang::Lexer("");
    auto tokStreamPtr = lexer.getTokenStream();
    auto pg = sammine_lang::Parser(tokStreamPtr);

    auto programAST = pg.Parse();

    REQUIRE(programAST->DefinitionVec.empty() == true);

}
