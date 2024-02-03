//
// Created by Jasmine Tang on 10/3/23.
//
#pragma once

#include <string>
#include <grammar.h>
#include <tao/pegtl.hpp>
#include <fstream>

namespace pegtl = TAO_PEGTL_NAMESPACE;


class Lexer {
public:
    std::vector<Token> tok_vector;
    size_t tok_pos = 0;
public:
    explicit Lexer(const std::string& file_name) {
        std::ifstream fl(file_name);
        if (fl.good()) {
            pegtl::file_input<> fi(file_name);
            pegtl::parse<sammine_lang::grammar, sammine_lang::action>(fi, tok_vector);
        } else {
            pegtl::string_input<> fi(file_name, "singular line");
            pegtl::parse<sammine_lang::grammar, sammine_lang::action>(fi, tok_vector);
        }

    }

    Token peek_current_token() {
        return tok_vector[tok_pos];
    }

    void consume_current_token() {
        tok_pos = std::min(tok_pos+1, tok_vector.size() - 1);
    }

    Token get_next_token() {
        auto tok = peek_current_token();
        consume_current_token();
        return tok;
    }
};
