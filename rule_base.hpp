//
//  rule.h
//  X3
//
//  Created by fudao on 2023/10/11.
//


#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <numeric>
#include <fstream>

#define BOOST_SPIRIT_DEBUG 1
//#define BOOST_SPIRIT_X3_DEBUG 1

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/fusion/include/io.hpp>

namespace parse {

namespace x3 = boost::spirit::x3;

struct base_variant_printer {
    typedef void result_type;
    result_type operator()(ast::nil) const { std::cout << "nil" << std::endl;}
    result_type operator()(const char& c) const { std::cout << "char:" << c << std::endl;}
    result_type operator()(const int& n) const { std::cout << "int:" << n << std::endl;}
    result_type operator()(const long& n) const { std::cout << "long:" << n << std::endl;}
    result_type operator()(const float& n) const { std::cout << "float:" << n << std::endl;}
    result_type operator()(const double& n) const { std::cout << "double:" << n << std::endl;}
    result_type operator()(const std::string& n) const { std::cout << "string:" << n << std::endl;}
    
    template<typename T>
    result_type operator()(const std::vector<T>& n) const {
        std::cout << "vector:" << std::endl;
        for(auto const& item : n){
            std::cout <<  ' ';
            (*this)(item);
        }
    }
    template<typename T>
    result_type operator()(const T& instance) const { boost::apply_visitor(*this, instance);}
};
static base_variant_printer base_var_printer;

struct x3_base_handle {
    template <typename Iterator, typename Exception, typename Context>
    x3::error_handler_result on_error(Iterator& first, Iterator const& last , Exception const& x, Context const& context){
        return x3::error_handler_result::fail;
    }
    template <typename T, typename Iterator, typename Context>
    inline void on_success(Iterator const& first, Iterator const& last, T& ast, Context const& context){
        
    }
};

struct x3_variant_handle : x3_base_handle{
    template <typename T, typename Iterator, typename Context>
    inline void on_success(Iterator const& first, Iterator const& last, T& ast, Context const& context){
        base_var_printer(ast);
    }
};




template <typename Tag>
static auto make_sym(std::initializer_list<char const*> elements) {
    x3::symbols<x3::unused_type> instance;
    for (auto el : elements)
        instance += el;
    return instance;
}
static auto const reserve_keywords = make_sym<struct reserve_keywords_class>({"int","long","short","double","char","unsigned","signed","const","void","volatile","enum","struct","union",
    "if","else","goto","switch","case","do","while","for","continue","break","return","default","typedef",
    "auto","register","extern","static",
    "sizeof"});



struct literal_string_class : x3_variant_handle{};
auto const literal_string = x3::rule<literal_string_class, std::string>{"literal_string"};
auto const literal_string_def = (x3::lexeme['"' >> +(x3::char_ - '"') >> '"']) /* | (x3::lexeme['\'' >> +(x3::char_ - '\'') >> '\''])*/;
BOOST_SPIRIT_DEFINE(literal_string);

struct identifier_class : x3_variant_handle{};
auto const  identifier = x3::rule<identifier_class,std::string>{"identifier"};
auto const identifier_def = x3::lexeme[(x3::alpha | x3::char_('_')) > *(x3::alnum | x3::char_('_'))] - reserve_keywords;
BOOST_SPIRIT_DEFINE(identifier);

}

