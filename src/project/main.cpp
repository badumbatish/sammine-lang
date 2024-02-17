// Copyright (c) 2014-2022 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <iostream>
#include <parser.h>
#include <string>
#include "Lexer.h"

#include "spdlog/spdlog.h"


int main(int argc, char* argv[]) {
    spdlog::set_pattern("[%^%l%$] [In: %s:%#] [Function %!] %v");
    SPDLOG_INFO("In function main()");
    SPDLOG_INFO("Checking if argument length is 1");
    if (argc == 1) {
        SPDLOG_ERROR("Argument length is 1. No arguments provided");
        return 1;
    }
    SPDLOG_INFO("Checking if argument length is 2");
    if (argc == 2) {
        Parser p((Lexer(argv[1])));
        p.parse();
        SPDLOG_INFO("Finish parsing");
        return 0;
    }

    SPDLOG_ERROR("Argument length is larger than 2.");
    SPDLOG_INFO("Invalid number of arguments. Use either no arguments or two arguments.");
    return 1;

}
