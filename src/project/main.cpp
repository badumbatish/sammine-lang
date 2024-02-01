// Copyright (c) 2014-2022 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <iostream>
#include <parser.h>
#include <string>
#include "Lexer.h"



int main() {
    Parser p(Lexer("toy.txt"));
    p.parse();
    return 0; // Exit successfully

}