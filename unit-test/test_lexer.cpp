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
    }

    SECTION( "Test lexeme") {
        REQUIRE( lex.peek().get()->lexeme == "a");
        lex.consume();

        REQUIRE( lex.peek().get()->lexeme == "2");
        lex.consume();
    }
}

TEST_CASE("Basic operators", "[Lexer]") {

    SECTION ("+ related tokens") {
        sammine_lang::Lexer lex("++ += +");
        REQUIRE(lex.consume().get()->type == sammine_lang::TokAddIncr);
        REQUIRE(lex.consume().get()->type == sammine_lang::TokAddAssign);
        REQUIRE(lex.consume().get()->type == sammine_lang::TokADD);
    }

    SECTION ("- related tokens") {
        sammine_lang::Lexer lex("-- -= -");
        REQUIRE(lex.consume().get()->type == sammine_lang::TokSubDecr);
        REQUIRE(lex.consume().get()->type == sammine_lang::TokSubAssign);
        REQUIRE(lex.consume().get()->type == sammine_lang::TokSUB);
    }

    SECTION ("* related tokens") {
        sammine_lang::Lexer lex("** *= -");
        REQUIRE(lex.consume().get()->type == sammine_lang::TokEXP);
        REQUIRE(lex.consume().get()->type == sammine_lang::TokMulAssign);
        REQUIRE(lex.consume().get()->type == sammine_lang::TokMUL);
    }

    SECTION ("/ related tokens") {
        sammine_lang::Lexer lex("/= /_ /^ /");
        REQUIRE(lex.consume().get()->type == sammine_lang::TokDivAssign);
        REQUIRE(lex.consume().get()->type == sammine_lang::TokFloorDiv);
        REQUIRE(lex.consume().get()->type == sammine_lang::TokCeilDiv);
        REQUIRE(lex.consume().get()->type == sammine_lang::TokDIV);
    }

    SECTION ("/ related tokens") {
        sammine_lang::Lexer lex("/= /_ /^ /");
        REQUIRE(lex.consume().get()->type == sammine_lang::TokDivAssign);
        REQUIRE(lex.consume().get()->type == sammine_lang::TokFloorDiv);
        REQUIRE(lex.consume().get()->type == sammine_lang::TokCeilDiv);
        REQUIRE(lex.consume().get()->type == sammine_lang::TokDIV);
    }

    SECTION ("% related tokens") {
        sammine_lang::Lexer lex("%");
        REQUIRE(lex.consume().get()->type == sammine_lang::TokMOD);
    }

    SECTION ("& related tokens") {
        sammine_lang::Lexer lex("&& &");
        REQUIRE(lex.consume().get()->type == sammine_lang::TokAND);
        REQUIRE(lex.consume().get()->type == sammine_lang::TokAndLogical);
    }

    SECTION ("| related tokens") {
        sammine_lang::Lexer lex("|| |");
        REQUIRE(lex.consume().get()->type == sammine_lang::TokOR);
        REQUIRE(lex.consume().get()->type == sammine_lang::TokORLogical);
    }

    SECTION ("^ related tokens") {
        sammine_lang::Lexer lex("^ ");
        REQUIRE(lex.consume().get()->type == sammine_lang::TokXOR);
    }

    SECTION ("<< >> related tokens") {
        sammine_lang::Lexer lex("<< >> ");
        REQUIRE(lex.consume().get()->type == sammine_lang::TokSHL);
        REQUIRE(lex.consume().get()->type == sammine_lang::TokSHR);

    }

    SECTION ("Basic comparision tokens") {
        sammine_lang::Lexer lex("== < > = !");

        REQUIRE(lex.consume().get()->type == sammine_lang::TokEQUAL);
        REQUIRE(lex.consume().get()->type == sammine_lang::TokLESS);
        REQUIRE(lex.consume().get()->type == sammine_lang::TokGREATER);
        REQUIRE(lex.consume().get()->type == sammine_lang::TokASSIGN);
        REQUIRE(lex.consume().get()->type == sammine_lang::TokNOT);
    }


}

TEST_CASE ("Basic utility tokens", "[Lexer]") {
    SECTION( "Parenthesis and curly tokens") {
        sammine_lang::Lexer lex("( ) { }");
        REQUIRE(lex.consume().get()->type == sammine_lang::TokLeftParen);
        REQUIRE(lex.consume().get()->type == sammine_lang::TokRightParen);
        REQUIRE(lex.consume().get()->type == sammine_lang::TokLeftCurly);
        REQUIRE(lex.consume().get()->type == sammine_lang::TokRightCurly);
    }

    SECTION (" Commas and Colons and pointing") {
        sammine_lang::Lexer lex(", . : :: ");
        REQUIRE(lex.consume().get()->type == sammine_lang::TokComma);
        REQUIRE(lex.consume().get()->type == sammine_lang::TokDot);
        REQUIRE(lex.consume().get()->type == sammine_lang::TokColon);
        REQUIRE(lex.consume().get()->type == sammine_lang::TokDoubleColon);
    }

    SECTION(" Comments") {
        sammine_lang::Lexer lex("# ");
        REQUIRE(lex.consume().get()->type == sammine_lang::TokSingleComment);
    }

    SECTION("EOF") {
        sammine_lang::Lexer lex("");
        REQUIRE(lex.consume().get()->type == sammine_lang::TokEOF);
    }
}