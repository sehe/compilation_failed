//
//  ast.h
//  X3
//
//  Created by fudao on 2023/10/11.
//
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <string>
#include <vector>

// #include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
// #include <boost/spirit/home/x3/support/utility/error_reporting.hpp>
// #include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>
#include <boost/fusion/include/adapted.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

namespace ast {

    namespace x3 = boost::spirit::x3;

    struct nil {};

    struct constant : x3::variant<intmax_t, double> {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct _pointer {
        char                 p;
        std::vector<uint8_t> type;
    };

    // using pointer = std::vector<_pointer>;
    using pointer = std::vector<_pointer>;

#if 1
    namespace test {

        struct unary_expression;
        struct conditional_expression;

        struct assignment_expression_1 {
            x3::forward_ast<unary_expression> unary;
            uint8_t                           oper;
        };
        struct assignment_expression {
            std::vector<assignment_expression_1>    prefix;
            x3::forward_ast<conditional_expression> exp;
        };

        using expression = std::vector<assignment_expression>;

        struct primary_expression : x3::variant<expression, constant, std::string, char> {
            using base_type::base_type;
            using base_type::operator=;
        };

        struct postfix_expression_1 : x3::variant<expression, std::string> {
            using base_type::base_type;
            using base_type::operator=;
        };
        struct postfix_expression {
            primary_expression                prefix;
            std::vector<postfix_expression_1> exp;
        };

        struct cast_expression;
        struct type_name;

        struct unary_expression_1 {
            uint8_t                          op;
            x3::forward_ast<cast_expression> exp;
        };
        struct unary_expression_2
            : x3::variant<nil, unary_expression_1, x3::forward_ast<type_name>, postfix_expression, char> {
            using base_type::base_type;
            using base_type::operator=;
        };
        struct unary_expression {
            std::vector<uint8_t> op;
            unary_expression_2   exp;
        };

        // using unary_expression= std::vector<uint8_t> ;

        struct cast_expression {
            std::vector<x3::forward_ast<type_name>> name;
            unary_expression                        exp;
        };

        template <typename expression> struct common_expression_last {
            uint8_t    op;
            expression exp;
        };
        template <typename expression> struct common_expression {
            using child = common_expression_last<expression>;
            expression         exp;
            std::vector<child> last;
        };

        using multiplicative_expression = common_expression<cast_expression>;
        using additive_expression       = common_expression<multiplicative_expression>;
        using shift_expression          = common_expression<additive_expression>;
        using relational_expression     = common_expression<shift_expression>;
        using equality_expression       = common_expression<relational_expression>;

        // struct and_expression{
        //     equality_expression pre;
        // };

        using and_expression          = std::vector<equality_expression>;
        using exclusive_or_expression = std::vector<and_expression>;
        using inclusive_or_expression = std::vector<exclusive_or_expression>;
        using logical_and_expression  = std::vector<inclusive_or_expression>;
        using logical_or_expression   = std::vector<logical_and_expression>;

        struct conditional_expression_1 {
            logical_or_expression one;
            expression            two;
        };
        struct conditional_expression {
            std::vector<conditional_expression_1> cond;
            logical_or_expression                 exp;
        };

        using constant_expression = conditional_expression;

        struct declarator;
        struct struct_declarator_1 {
            x3::forward_ast<declarator> pre;
            constant_expression         last;
        };
        struct struct_declarator
            : x3::variant<struct_declarator_1, constant_expression, x3::forward_ast<declarator>> {
            using base_type::base_type;
            using base_type::operator=;
        };

        struct struct_declarator_list_1 {
            struct_declarator   pre;
            constant_expression exp;
        };
        struct struct_declarator_list
            : x3::variant<std::vector<struct_declarator_list_1>, std::vector<struct_declarator>> {
            using base_type::base_type;
            using base_type::operator=;
        };

        struct enumerator_1 {
            std::string         iden;
            constant_expression exp;
        };
        struct enumerator : x3::variant<enumerator_1, std::string> {
            using base_type::base_type;
            using base_type::operator=;
        };

        using typedef_name    = std::string;
        using enumerator_list = std::vector<enumerator>;

        struct enum_specifier_1 {
            std::string     ident;
            enumerator_list list;
        };

        struct enum_specifier : x3::variant<enum_specifier_1, enumerator_list, std::string> {
            using base_type::base_type;
            using base_type::operator=;
        };

        struct type_specifier;
        struct specifier_qualifier : x3::variant<x3::forward_ast<type_specifier>, uint8_t> {
            using base_type::base_type;
            using base_type::operator=;
        };

        struct struct_declaration {
            std::vector<specifier_qualifier> pre;
            struct_declarator_list           list;
        };

        struct struct_or_union_specifier_1 {
            uint8_t                         t;
            std::string                     ident;
            std::vector<struct_declaration> list;
        };
        struct struct_or_union_specifier_2 {
            uint8_t                         t;
            std::vector<struct_declaration> list;
        };
        struct struct_or_union_specifier_3 {
            uint8_t     t;
            std::string ident;
        };
        struct struct_or_union_specifier
            : x3::variant<struct_or_union_specifier_1, struct_or_union_specifier_2,
                          struct_or_union_specifier_3> {
            using base_type::base_type;
            using base_type::operator=;
        };

        struct type_specifier
            : x3::variant<uint8_t, struct_or_union_specifier, enum_specifier, typedef_name> {
            using base_type::base_type;
            using base_type::operator=;
        };

        struct declaration_specifier : x3::variant<uint8_t, type_specifier, typedef_name> {
            using base_type::base_type;
            using base_type::operator=;
        };

        struct initializer;
        using initializer_list = std::vector<x3::forward_ast<initializer>>;

        struct initializer : x3::variant<boost::recursive_wrapper<initializer_list>, assignment_expression> {
            using base_type::base_type;
            using base_type::operator=;
        };

        struct init_declarator_1 {
            x3::forward_ast<declarator> pre;
            initializer                 last;
        };
        struct init_declarator : x3::variant<init_declarator_1, x3::forward_ast<declarator>> {
            using base_type::base_type;
            using base_type::operator=;
        };

        struct declaration;
        struct statement;
        struct compound_statement {
            std::vector<x3::forward_ast<declaration>> pre;
            std::vector<x3::forward_ast<statement>>   last;
        };

        struct labeled_statement_1 {
            std::string                pre;
            x3::forward_ast<statement> last;
        };
        struct labeled_statement_2 {
            constant_expression        pre;
            x3::forward_ast<statement> last;
        };
        struct labeled_statement
            : x3::variant<x3::forward_ast<statement>, labeled_statement_2, labeled_statement_1> {
            using base_type::base_type;
            using base_type::operator=;
        };

        using expression_statement = boost::optional<expression>;

        struct selection_statement_0 {
            x3::forward_ast<statement> pre;
            x3::forward_ast<statement> last;
        };

        struct selection_statement_1 : x3::variant<selection_statement_0, x3::forward_ast<statement>> {
            using base_type::base_type;
            using base_type::operator=;
        };
        struct selection_statement_2 {
            expression            pre;
            selection_statement_1 last;
        };
        struct selection_statement_3 {
            expression                 pre;
            x3::forward_ast<statement> last;
        };
        struct selection_statement : x3::variant<selection_statement_2, selection_statement_3> {
            using base_type::base_type;
            using base_type::operator=;
        };

        struct _iteration_statement1 {
            expression                 exp;
            x3::forward_ast<statement> stat;
        };
        struct _iteration_statement2 {
            x3::forward_ast<statement> stat;
            expression                 exp;
        };
        struct _iteration_statement3 {
            boost::optional<expression> a;
            boost::optional<expression> b;
            boost::optional<expression> c;
            x3::forward_ast<statement>  stat;
        };

        struct iteration_statement
            : x3::variant<nil, _iteration_statement1, _iteration_statement2, _iteration_statement3> {
            using base_type::base_type;
            using base_type::operator=;
        };

        struct jump_statement : x3::variant<nil, boost::optional<expression>, std::string> {
            using base_type::base_type;
            using base_type::operator=;
        };

        struct statement
            : x3::variant<boost::recursive_wrapper<labeled_statement>, expression_statement,
                          boost::recursive_wrapper<compound_statement>,
                          boost::recursive_wrapper<selection_statement>,
                          boost::recursive_wrapper<iteration_statement>, jump_statement> {
            using base_type::base_type;
            using base_type::operator=;
        };

        struct declaration {
            std::vector<declaration_specifier> pre;
            std::vector<init_declarator>       laster;
        };

        struct abstract_declarator;
        struct parameter_declaration;

        // using parameter_type_list = std::vector<x3::forward_ast<parameter_declaration>>;
        using parameter_type_list = std::vector<x3::forward_ast<parameter_declaration>>;
        // using parameter_type_list= parameter_list ;

        struct _direct_abstract_declarator
            : x3::variant<boost::optional<constant_expression>, boost::optional<parameter_type_list>> {
            using base_type::base_type;
            using base_type::operator=;
        };
        struct direct_abstract_declarator {
            x3::forward_ast<abstract_declarator>     pre;
            std::vector<_direct_abstract_declarator> list;
        };

        struct abstract_declarator_1 {
            pointer                    p;
            direct_abstract_declarator last;
        };

        struct abstract_declarator : x3::variant<abstract_declarator_1, pointer, direct_abstract_declarator> {
            using base_type::base_type;
            using base_type::operator=;
        };

        struct parameter_declaration_1 : x3::variant<x3::forward_ast<declarator>, abstract_declarator> {
            using base_type::base_type;
            using base_type::operator=;
        };
        struct parameter_declaration {
            std::vector<declaration_specifier>       pre;
            boost::optional<parameter_declaration_1> last;
        };

        // using parameter_list = std::vector<parameter_declaration>;
        using parameter_list = std::vector<parameter_declaration>;

        struct type_name {
            std::vector<specifier_qualifier>     pre;
            boost::optional<abstract_declarator> last;
        };

        struct direct_declarator;
        struct declarator {
            boost::optional<pointer>           p;
            x3::forward_ast<direct_declarator> last;
        };

        struct _direct_declarator1 : x3::variant<std::string, boost::recursive_wrapper<declarator>> {
            using base_type::base_type;
            using base_type::operator=;
        };

        struct _direct_declarator2
            : x3::variant<boost::optional<constant_expression>, parameter_type_list,
                          std::vector<std::string>> {
            using base_type::base_type;
            using base_type::operator=;
        };

        struct direct_declarator {
            _direct_declarator1              pre;
            std::vector<_direct_declarator2> last;
        };

        struct function_definition {
            std::vector<declaration_specifier> pre;
            declarator                         mid;
            std::vector<declaration>           mid2;
            compound_statement                 last;
        };

        struct external_declaration : x3::variant<function_definition, declaration> {
            using base_type::base_type;
            using base_type::operator=;
        };

        using boost::fusion::operator<<;
    } // namespace test
#endif

    // main
    using translation_unit = std::vector<test::external_declaration>;

    static inline std::ostream& operator<<(std::ostream& out, nil) { return out << "nil"; }
    using boost::fusion::operator<<;
} // namespace ast

BOOST_FUSION_ADAPT_STRUCT(ast::_pointer, p, type)
#if 0
    BOOST_FUSION_ADAPT_STRUCT(ast::additive_expression_last, s, exp)
    BOOST_FUSION_ADAPT_STRUCT(ast::additive_expression, exp, last)

    BOOST_FUSION_ADAPT_STRUCT(ast::shift_expression_last, s, exp)
    BOOST_FUSION_ADAPT_STRUCT(ast::shift_expression, exp, last)
#endif

#if 1
    BOOST_FUSION_ADAPT_STRUCT(ast::test::assignment_expression_1, unary, oper)
    BOOST_FUSION_ADAPT_STRUCT(ast::test::assignment_expression, prefix, exp)

    BOOST_FUSION_ADAPT_STRUCT(ast::test::postfix_expression, prefix, exp)

    BOOST_FUSION_ADAPT_STRUCT(ast::test::unary_expression_1, op, exp)
    BOOST_FUSION_ADAPT_STRUCT(ast::test::unary_expression, op, exp)

    BOOST_FUSION_ADAPT_STRUCT(ast::test::cast_expression, name, exp)

    BOOST_FUSION_ADAPT_STRUCT(ast::test::multiplicative_expression::child, op, exp)
    BOOST_FUSION_ADAPT_STRUCT(ast::test::multiplicative_expression, exp, last)

    BOOST_FUSION_ADAPT_STRUCT(ast::test::additive_expression::child, op, exp)
    BOOST_FUSION_ADAPT_STRUCT(ast::test::additive_expression, exp, last)

    BOOST_FUSION_ADAPT_STRUCT(ast::test::shift_expression::child, op, exp)
    BOOST_FUSION_ADAPT_STRUCT(ast::test::shift_expression, exp, last)

    BOOST_FUSION_ADAPT_STRUCT(ast::test::relational_expression::child, op, exp)
    BOOST_FUSION_ADAPT_STRUCT(ast::test::relational_expression, exp, last)

    BOOST_FUSION_ADAPT_STRUCT(ast::test::equality_expression::child, op, exp)
    BOOST_FUSION_ADAPT_STRUCT(ast::test::equality_expression, exp, last)

    BOOST_FUSION_ADAPT_STRUCT(ast::test::conditional_expression_1, one, two)
    BOOST_FUSION_ADAPT_STRUCT(ast::test::conditional_expression, cond, exp)

    BOOST_FUSION_ADAPT_STRUCT(ast::test::struct_declarator_1, pre, last)

    BOOST_FUSION_ADAPT_STRUCT(ast::test::struct_declarator_list_1, pre, exp)

    BOOST_FUSION_ADAPT_STRUCT(ast::test::enumerator_1, iden, exp)

    BOOST_FUSION_ADAPT_STRUCT(ast::test::enum_specifier_1, ident, list)

    BOOST_FUSION_ADAPT_STRUCT(ast::test::struct_declaration, pre, list)

    BOOST_FUSION_ADAPT_STRUCT(ast::test::struct_or_union_specifier_1, t, ident, list)
    BOOST_FUSION_ADAPT_STRUCT(ast::test::struct_or_union_specifier_2, t, list)
    BOOST_FUSION_ADAPT_STRUCT(ast::test::struct_or_union_specifier_3, t, ident)

    BOOST_FUSION_ADAPT_STRUCT(ast::test::init_declarator_1, pre, last)

    BOOST_FUSION_ADAPT_STRUCT(ast::test::compound_statement, pre, last)

    BOOST_FUSION_ADAPT_STRUCT(ast::test::labeled_statement_1, pre, last)
    BOOST_FUSION_ADAPT_STRUCT(ast::test::labeled_statement_2, pre, last)

    BOOST_FUSION_ADAPT_STRUCT(ast::test::selection_statement_0, pre, last)
    BOOST_FUSION_ADAPT_STRUCT(ast::test::selection_statement_2, pre, last)
    BOOST_FUSION_ADAPT_STRUCT(ast::test::selection_statement_3, pre, last)

    BOOST_FUSION_ADAPT_STRUCT(ast::test::_iteration_statement1, exp, stat)
    BOOST_FUSION_ADAPT_STRUCT(ast::test::_iteration_statement2, stat, exp)
    BOOST_FUSION_ADAPT_STRUCT(ast::test::_iteration_statement3, a, b, c, stat)

    BOOST_FUSION_ADAPT_STRUCT(ast::test::declaration, pre, laster)

    BOOST_FUSION_ADAPT_STRUCT(ast::test::direct_abstract_declarator, pre, list)

    BOOST_FUSION_ADAPT_STRUCT(ast::test::abstract_declarator_1, p, last)

    BOOST_FUSION_ADAPT_STRUCT(ast::test::parameter_declaration, pre, last)
    BOOST_FUSION_ADAPT_STRUCT(ast::test::type_name, pre, last)
    BOOST_FUSION_ADAPT_STRUCT(ast::test::declarator, p, last)

    BOOST_FUSION_ADAPT_STRUCT(ast::test::direct_declarator, pre, last)

    BOOST_FUSION_ADAPT_STRUCT(ast::test::function_definition, pre, mid, mid2, last)
#endif
