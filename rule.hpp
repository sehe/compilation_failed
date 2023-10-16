//
//  rule.h
//  X3
//
//  Created by fudao on 2023/10/11.
//
#ifndef rule_h
#define rule_h

#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <string>
#include <vector>

// #include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
// #include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>
// #include <boost/spirit/home/x3/support/utility/error_reporting.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

#include "rule_base.hpp"

namespace parse {

    typedef std::string::const_iterator iterator_type;

    namespace x3 = boost::spirit::x3;

    struct typedef_name_class : x3_variant_handle {};
    struct unary_operator_class            : x3_variant_handle        {};
    struct type_qualifier_class            : x3_variant_handle        {};
    struct pointer_class                   : parse::x3_base_handle    {};
    struct storage_class_specifier_class   : x3_variant_handle        {};
    struct struct_or_union_class           : parse::x3_variant_handle {};
#if 0
    struct function_definition_class       : parse::x3_base_handle{};
    struct multiplicative_expression_class : parse::x3_base_handle{};
    struct additive_expression_class       : parse::x3_base_handle{};
    struct shift_expression_class          : parse::x3_base_handle{};
#endif

    struct translation_unit_class : x3_base_handle {};
    auto const translation_unit = x3::rule<translation_unit_class, ast::translation_unit>{"translation_unit"};

    auto const typedef_name = x3::rule<typedef_name_class, std::string>{"typedef_name"};

#if 0
    auto const unary_operator  = x3::rule<unary_operator_class, char>{"unary_operator"};
    auto const type_qualifier  = x3::rule<type_qualifier_class, std::string>{"type_qualifier"};
    auto const struct_or_union = x3::rule<struct_or_union_class,std::string>{"struct_or_union"};
    auto const storage_class_specifier =
        x3::rule<storage_class_specifier_class, std::string>{"storage_class_specifier"};
    auto const assignment_operator =
        x3::rule<x3_variant_handle, ast::assignment_operator>{"assignment_operator"};
#endif

    auto const unary_operator           = x3::symbols<uint8_t>{"unary_operator"};
    auto const type_qualifier           = x3::symbols<uint8_t>{"type_qualifier"};
    auto const struct_or_union          = x3::symbols<uint8_t>{"struct_or_union"};
    auto const storage_class_specifier  = x3::symbols<uint8_t>{"storage_class_specifier"};
    auto const assignment_operator      = x3::symbols<uint8_t>{"assignment_operator"};

    /// base oper
    auto const type_specifier_pre       = x3::symbols<uint8_t>{"type_specifier_pre"};
    auto const multiplicative_oper      = x3::symbols<uint8_t>{"multiplicative_oper"};
    auto const additive_oper            = x3::symbols<uint8_t>{"additive_oper"};
    auto const shift_oper               = x3::symbols<uint8_t>{"shift_oper"};
    auto const relational_oper          = x3::symbols<uint8_t>{"relational_oper"};
    auto const equality_expression_oper = x3::symbols<uint8_t>{"equality_expression_oper"};
    auto const unary_expression_oper    = x3::symbols<uint8_t>{"unary_expression_oper"};
    auto const jump_oper                = x3::symbols<uint8_t>{"jump_oper"};
    /// base oper

    auto const pointer                  = x3::rule<pointer_class, ast::pointer>{"pointer"};
    auto const constant                 = x3::rule<x3_variant_handle, ast::constant>{"constant"};

#if 0
    auto const declarator = x3::rule<x3_base_handle, Pointer_Type>{"declarator"};
    auto const multiplicative_expression =
        x3::rule<multiplicative_expression_class, ast::multiplicative_expression>{
            "multiplicative_expression"};
    auto const additive_expression =
        x3::rule<additive_expression_class, ast::additive_expression>{"additive_expression"};
    auto const shift_expression = x3::rule<shift_expression_class, ast::shift_expression>{"shift_expression"};
#endif

    static inline void add_keywords() {
        static bool once = false;
        if (once)
            return;
        once = true;

        unary_operator.add("&", 1)("*", 2)("+", 3)("-", 4)("~", 5)("!", 6);
        type_qualifier.add("const", 1)("volatile", 2);
        struct_or_union.add("struct", 1)("union", 2);
        storage_class_specifier.add("auto", 1)("register", 2)("static", 3)("extern", 4)("typedef", 5);
        type_specifier_pre.add("void", 1)("char", 2)("short", 3)("int", 4)("long", 5)("float", 6)(
            "double", 7)("signed", 8)("unsigned", 9);
        assignment_operator.add("*=", 1)("/=", 2)("%=", 3)("+=", 4)("-=", 5)("<<=", 6)(">>=", 7)("&=", 8)(
            "^=", 9)("|=", 10)("=", 11);
        multiplicative_oper.add("*", 1)("/", 2);
        additive_oper.add("+", 1)("-", 2);
        shift_oper.add("<<", 1)(">>", 2);
        relational_oper.add(">", 1)("<", 2)(">=", 1)("<=", 2);
        equality_expression_oper.add("==", 1)("!=", 2);
        unary_expression_oper.add("sizeof", 1)("--", 2)("++", 3);
        jump_oper.add("return", 1)("break", 2)("continue", 3)("goto", 4);
    }

    static inline auto const main_rule() {
        add_keywords();
        return translation_unit;
    }
} // namespace parse

#define BNF_DEC_HANDLE(x, handle)                                                                            \
    namespace ast {                                                                                          \
        using x = x3::unused_type;                                                                           \
    }                                                                                                        \
    namespace parse {                                                                                        \
        struct x##_class : handle {};                                                                        \
    }                                                                                                        \
    namespace parse {                                                                                        \
        auto const x = x3::rule<x##_class, ast::test::x>{#x};                                                \
    }

#define BNF_QUIC_DEC(x)    BNF_DEC_HANDLE(x, x3_base_handle)
#define BNF_DEFAULT_DEC(x) BNF_DEC_HANDLE(x, x##_handle)

// BNF_QUIC_DEC(translation_unit)
BNF_QUIC_DEC(external_declaration)
BNF_QUIC_DEC(function_definition)
BNF_QUIC_DEC(declaration_specifier)

// BNF_QUIC_DEC(storage_class_specifier)
// BNF_QUIC_DEC(type_qualifier)
// BNF_QUIC_DEC(pointer)

BNF_QUIC_DEC(type_specifier)
BNF_QUIC_DEC(struct_or_union_specifier)

// BNF_QUIC_DEC(struct_or_union)

BNF_QUIC_DEC(struct_declaration)
BNF_QUIC_DEC(specifier_qualifier)
BNF_QUIC_DEC(struct_declarator_list)
BNF_QUIC_DEC(struct_declarator)
BNF_QUIC_DEC(declarator)

BNF_QUIC_DEC(direct_declarator)
BNF_QUIC_DEC(constant_expression)
BNF_QUIC_DEC(conditional_expression)
BNF_QUIC_DEC(logical_or_expression)
BNF_QUIC_DEC(logical_and_expression)
BNF_QUIC_DEC(inclusive_or_expression)
BNF_QUIC_DEC(exclusive_or_expression)
BNF_QUIC_DEC(and_expression)
BNF_QUIC_DEC(equality_expression)
BNF_QUIC_DEC(relational_expression)
BNF_QUIC_DEC(shift_expression)
BNF_QUIC_DEC(additive_expression)
BNF_QUIC_DEC(multiplicative_expression)
BNF_QUIC_DEC(cast_expression)
BNF_QUIC_DEC(unary_expression)
BNF_QUIC_DEC(postfix_expression)
BNF_QUIC_DEC(primary_expression)

BNF_QUIC_DEC(expression)
BNF_QUIC_DEC(assignment_expression)

// BNF_QUIC_DEC(unary_operator)
BNF_QUIC_DEC(type_name)
BNF_QUIC_DEC(parameter_type_list)
BNF_QUIC_DEC(parameter_list)
BNF_QUIC_DEC(parameter_declaration)
BNF_QUIC_DEC(abstract_declarator)
BNF_QUIC_DEC(direct_abstract_declarator)
BNF_QUIC_DEC(enum_specifier)
BNF_QUIC_DEC(enumerator_list)
BNF_QUIC_DEC(enumerator)
//BNF_DEFAULT_DEC(typedef_name)
BNF_QUIC_DEC(declaration)
BNF_QUIC_DEC(init_declarator)
BNF_QUIC_DEC(initializer)
BNF_QUIC_DEC(initializer_list)
BNF_QUIC_DEC(compound_statement)
BNF_QUIC_DEC(statement)
BNF_QUIC_DEC(labeled_statement)
BNF_QUIC_DEC(expression_statement)
BNF_QUIC_DEC(selection_statement)
BNF_QUIC_DEC(iteration_statement)
BNF_QUIC_DEC(jump_statement)


#endif /* rule_h */
