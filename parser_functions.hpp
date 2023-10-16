#pragma once

#include "rule_def.hpp"

#define TEST_FUNCTION(x)                                                                                     \
    static inline bool test_##x(std::string_view src) {                                                      \
        ast::x out;                         \
        return phrase_parse(                \
                src.cbegin(), src.cend(),   \
                parse::x >> parse::x3::eoi, \
                parse::x3::space,           \
                out);                       \
    }

namespace parse::tester {
    TEST_FUNCTION(translation_unit)
    TEST_FUNCTION(external_declaration)
    TEST_FUNCTION(function_definition)
    TEST_FUNCTION(declaration_specifier)
    // TEST_FUNCTION(storage_class_specifier) // TODO ast type
    // TEST_FUNCTION(type_qualifier) //          TODO ast type
    TEST_FUNCTION(pointer)
    TEST_FUNCTION(type_specifier)
    TEST_FUNCTION(struct_or_union_specifier)
    // TEST_FUNCTION(struct_or_union)
    TEST_FUNCTION(struct_declaration)
    TEST_FUNCTION(specifier_qualifier)
    TEST_FUNCTION(struct_declarator_list)
    TEST_FUNCTION(struct_declarator)
    TEST_FUNCTION(declarator)
    TEST_FUNCTION(direct_declarator)
    // TEST_FUNCTION(constant_expression) //     TODO BOOST_SPIRIT_DEFINE
    TEST_FUNCTION(conditional_expression)
    TEST_FUNCTION(logical_or_expression)
    TEST_FUNCTION(logical_and_expression)
    TEST_FUNCTION(inclusive_or_expression)
    TEST_FUNCTION(exclusive_or_expression)
    TEST_FUNCTION(and_expression)
    TEST_FUNCTION(equality_expression)
    TEST_FUNCTION(relational_expression)
    TEST_FUNCTION(shift_expression)
    TEST_FUNCTION(additive_expression)
    TEST_FUNCTION(multiplicative_expression)
    TEST_FUNCTION(cast_expression)
    TEST_FUNCTION(unary_expression)
    TEST_FUNCTION(postfix_expression)
    TEST_FUNCTION(primary_expression)
    TEST_FUNCTION(expression)
    TEST_FUNCTION(assignment_expression)
    // TEST_FUNCTION(unary_operator) //          TODO ast type
    TEST_FUNCTION(type_name)
    TEST_FUNCTION(parameter_type_list)
    TEST_FUNCTION(parameter_list)
    TEST_FUNCTION(parameter_declaration)
    TEST_FUNCTION(abstract_declarator)
    TEST_FUNCTION(direct_abstract_declarator)
    TEST_FUNCTION(enum_specifier)
    TEST_FUNCTION(enumerator_list)
    TEST_FUNCTION(enumerator)
    // TEST_FUNCTION(typedef_name) //            TODO ast type
    TEST_FUNCTION(declaration)
    TEST_FUNCTION(init_declarator)
    TEST_FUNCTION(initializer)
    TEST_FUNCTION(initializer_list)
    TEST_FUNCTION(compound_statement)
    TEST_FUNCTION(statement)
    TEST_FUNCTION(labeled_statement)
    TEST_FUNCTION(expression_statement)
    TEST_FUNCTION(selection_statement)
    TEST_FUNCTION(iteration_statement)
    TEST_FUNCTION(jump_statement)
} // namespace parse::tester
