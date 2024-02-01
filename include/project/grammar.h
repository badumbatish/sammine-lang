//
// Created by jjsm on 1/31/24.
//

#ifndef SAMMINE_LANG_GRAMMAR_H
#define SAMMINE_LANG_GRAMMAR_H

#include <tao/pegtl.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;
enum TokenType {
    tok_eof,

    // commands
    tok_def,
    tok_extern,

    // primary
    tok_identifier,
    tok_number,

    tok_semicolon,
    tok_epar,
    tok_opar,
    tok_comma,
    tok_binop,
};

using Token = std::pair<TokenType, std::string>;

namespace sammine_lang {
    struct tok_eof : pegtl::eof {};
    struct tok_def : TAO_PEGTL_STRING( "def" ) {};
    struct tok_extern : TAO_PEGTL_STRING( "extern" ) {};
    struct tok_number : pegtl::plus<pegtl::digit> {};
    struct tok_identifier : pegtl::identifier{};
    struct tok_whitespace : pegtl::space{};
    struct tok_semicolon : TAO_PEGTL_STRING( ";" ) {};
    struct tok_opar : TAO_PEGTL_STRING("(") {};
    struct tok_epar : TAO_PEGTL_STRING(")") {};
    struct tok_comma : TAO_PEGTL_STRING(",") {};
    struct tok_binop : pegtl::one<'+', '-', '*', '<'> {};
    struct expression : pegtl::sor<tok_def, tok_extern, tok_number, tok_identifier, tok_whitespace, tok_opar, tok_epar,tok_semicolon, tok_comma, tok_binop>{};
    struct grammar : pegtl::seq<pegtl::plus<expression>, tok_eof>{};
    template< typename Rule >
    struct action
    {};

    template<>
    struct action<tok_eof> {
        static void apply0(std::vector<Token> &tok_vector) {
            tok_vector.emplace_back(TokenType::tok_eof, "eof");
        }
    };

    template<>
    struct action<tok_def> {
        static void apply0(std::vector<Token> &tok_vector) {
            tok_vector.emplace_back(TokenType::tok_def, "def");
        }
    };

    template<>
    struct action<tok_extern> {
        static void apply0(std::vector<Token> &tok_vector) {
            tok_vector.emplace_back(TokenType::tok_extern, "extern");
        }
    };

    template<>
    struct action<tok_number> {
        template<typename ActionInput>
        static void apply(const ActionInput& in, std::vector<Token>& tok_vector) {
            tok_vector.push_back({TokenType::tok_number, in.string()});
        }
    };

    template<>
    struct action<tok_identifier> {
        template<typename ActionInput>
        static void apply(const ActionInput& in, std::vector<Token>& tok_vector) {
            tok_vector.push_back({TokenType::tok_identifier, in.string()});
        }
    };

    template<>
    struct action<tok_semicolon> {
        template<typename ActionInput>
        static void apply(const ActionInput& in, std::vector<Token>& tok_vector) {
            tok_vector.push_back({TokenType::tok_semicolon, in.string()});
        }
    };

    template<>
    struct action<tok_epar> {
        template<typename ActionInput>
        static void apply(const ActionInput& in, std::vector<Token>& tok_vector) {
            tok_vector.push_back({TokenType::tok_epar, in.string()});
        }
    };

    template<>
    struct action<tok_opar> {
        template<typename ActionInput>
        static void apply(const ActionInput& in, std::vector<Token>& tok_vector) {
            tok_vector.push_back({TokenType::tok_opar, in.string()});
        }
    };

    template<>
    struct action<tok_comma> {
        template<typename ActionInput>
        static void apply(const ActionInput& in, std::vector<Token>& tok_vector) {
            tok_vector.push_back({TokenType::tok_comma, in.string()});
        }
    };

    template<>
    struct action<tok_binop> {
        template<typename ActionInput>
        static void apply(const ActionInput& in, std::vector<Token>& tok_vector) {
            tok_vector.push_back({TokenType::tok_binop, in.string()});
        }
    };
}


#endif //SAMMINE_LANG_GRAMMAR_H
