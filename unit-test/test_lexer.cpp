//
// Created by jjasmine on 3/8/24.
//

#include <catch2/catch_test_macros.hpp>
#include "Lexer.h"

TEST_CASE( "hello (lex) world", "[Lexer]" ) {
    sammine_lang::Lexer lex("a 2");
    REQUIRE( lex.peek().get()->type == sammine_lang::TokID );
    lex.consume();
    REQUIRE( lex.peek().get()->type == sammine_lang::TokNum );
    lex.consume();
}