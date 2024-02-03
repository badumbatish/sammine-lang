// Copyright (c) 2014-2022 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <iostream>
#include <parser.h>
#include <string>
#include "Lexer.h"


int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cout << "No arguments provided." << std::endl;
    } else if (argc == 2) {
        Parser p((Lexer(argv[1])));
        p.parse();
    } else {
        std::cout << "Invalid number of arguments. Use either no arguments or two arguments." << std::endl;
    }

    return 0;
}
