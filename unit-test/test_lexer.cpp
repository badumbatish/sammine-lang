//
// Created by jjasmine on 3/8/24.
//

//! \file test_lexer.cpp
//! \brief The unit-test file for all things related to a lexer.
#include <catch2/catch_test_macros.hpp>
#include "Lexer.h"

//! Simple test cases for a Lexer, test for an identifier followed by a number (both of length 1)
TEST_CASE( "hello (lex) world", "[Lexer]" ) {
    sammine_lang::Lexer lex("a 2");

    SECTION( "Test token type") {
        REQUIRE( lex.peek().get()->type == sammine_lang::TokID );
        lex.consume();

        REQUIRE( lex.peek().get()->type == sammine_lang::TokNum );
        lex.consume();

        REQUIRE( lex.peek().get()->type == sammine_lang::TokEOF );
        lex.consume();

    }

    SECTION( "Test lexeme") {
        REQUIRE( lex.peek().get()->lexeme == "a");
        lex.consume();

        REQUIRE( lex.peek().get()->lexeme == "2");
        lex.consume();
    }
}

