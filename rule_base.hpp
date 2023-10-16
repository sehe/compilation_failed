//
//  rule.h
//  X3
//
//  Created by fudao on 2023/10/11.
//
#ifndef rule_base_hpp
#define rule_base_hpp

#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <string>
#include <vector>

//#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
//#include <boost/spirit/home/x3/support/ast/variant.hpp>
//#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>
//#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>
#include <boost/fusion/include/adapted.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/spirit/home/x3.hpp>

#include "ast_base.hpp"

namespace parse {
    namespace x3 = boost::spirit::x3;

    struct base_variant_printer {
        using result_type = void;
        void operator()(ast::nil)   const           { std::cout << "nil"     << std::endl; }  
        void operator()(char        const& c) const { std::cout << "char:"   << c << std::endl; } 
        void operator()(int         const& n) const { std::cout << "int:"    << n << std::endl; } 
        void operator()(long        const& n) const { std::cout << "long:"   << n << std::endl; } 
        void operator()(float       const& n) const { std::cout << "float:"  << n << std::endl; } 
        void operator()(double      const& n) const { std::cout << "double:" << n << std::endl; } 
        void operator()(std::string const& n) const { std::cout << "string:" << n << std::endl; } 

        template <typename T> void operator()(std::vector<T> const& n) const {
            std::cout << "vector:" << std::endl;
            for (auto const& item : n) {
                std::cout << ' ';
                (*this)(item);
            }
        }
        template <typename T> void operator()(T const& instance) const {
            boost::apply_visitor(*this, instance);
        }
    };

    struct x3_base_handle {
        template <typename It, typename E, typename Ctx>
        auto on_error(It& first, It const& last, E const& x, Ctx const& ctx) {
            return x3::error_handler_result::fail;
        }
        template <typename T, typename It, typename Ctx>
        inline void on_success(It const& first, It const& last, T& ast, Ctx const& ctx) {}
    };

    struct x3_variant_handle : x3_base_handle {
        template <typename T, typename It, typename Ctx>
        inline void on_success(It const& first, It const& last, T& ast, Ctx const& ctx) {
            base_variant_printer{}(ast);
        }
    };

    template <typename Tag> static auto make_sym(std::initializer_list<char const*> elements) {
        x3::symbols<x3::unused_type> instance;
        for (auto el : elements)
            instance += el;
        return instance;
    }
    static auto const reserve_keywords = make_sym<struct reserve_keywords_class>(
        {"int",     "long",     "short", "double",   "char",   "unsigned", "signed", "const",
         "void",    "volatile", "enum",  "struct",   "union",  "if",       "else",   "goto",
         "switch",  "case",     "do",    "while",    "for",    "continue", "break",  "return",
         "default", "typedef",  "auto",  "register", "extern", "static",   "sizeof"});

    struct literal_string_class : x3_variant_handle {};
    auto const literal_string = x3::rule<literal_string_class, std::string>{"literal_string"};
    auto const literal_string_def =
        (x3::lexeme['"' >> +~x3::char_('"') >> '"']) /* | (x3::lexeme['\'' >> +(x3::char_ - '\'') >> '\''])*/;
    BOOST_SPIRIT_DEFINE(literal_string)

    struct identifier_class : x3_variant_handle {};
    auto const identifier = x3::rule<identifier_class, std::string>{"identifier"};
    auto const identifier_def =
        x3::lexeme[(x3::alpha | x3::char_('_')) > *(x3::alnum | x3::char_('_'))] - reserve_keywords;
    BOOST_SPIRIT_DEFINE(identifier)
} // namespace parse

#endif // rule_base_hpp
