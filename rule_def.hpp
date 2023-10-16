//
//  rule_def.hpp
//  X3
//
//  Created by fudao on 2023/10/11.
//

#ifndef rule_def_h
#define rule_def_h

#include "rule.hpp"

namespace parse {
namespace x3 = boost::spirit::x3;

auto const translation_unit_def = *(external_declaration);
auto const external_declaration_def = function_definition
| declaration;
auto const function_definition_def = *declaration_specifier >> declarator >> *declaration >> compound_statement;
auto const declaration_specifier_def = storage_class_specifier
| type_specifier
| type_qualifier;
auto const storage_class_specifier_def = x3::string("auto") | x3::string("register") | x3::string("static") | x3::string("extern") | x3::string("typedef");
auto const type_specifier_def = type_specifier_pre
| struct_or_union_specifier
| enum_specifier
| typedef_name;

auto const struct_or_union_specifier_def = (struct_or_union >>  identifier >> '{' >>  +struct_declaration >> '}')
| (struct_or_union >> '{' >> +struct_declaration >> '}')
| (struct_or_union >>  identifier) ;
auto const struct_or_union_def = x3::string("struct")
| x3::string("union");
auto const struct_declaration_def = *specifier_qualifier >> struct_declarator_list;
auto const specifier_qualifier_def = type_specifier
| type_qualifier;

// custom change
//auto const struct_declarator_list_def = struct_declarator % ',';
auto const struct_declarator_list_def = (((struct_declarator  >> ('=' >> conditional_expression)) % ';') >> ';')
| ((struct_declarator % ';') >> ';');


auto const struct_declarator_def = (declarator >> ':' >> conditional_expression)
| ':' >> conditional_expression
| declarator;


auto const declarator_def = -pointer >> direct_declarator;
auto const pointer_def = +(x3::char_('*') >> *type_qualifier);
auto const type_qualifier_def = x3::string("const")
| x3::string("volatile");

auto const direct_declarator_def = ( identifier | ('(' >> declarator >> ')')) >> *((x3::lit('[') >> -conditional_expression >> ']') | (x3::lit('(') >> parameter_type_list >> ')') | (x3::lit('(') >> *identifier >> ')'));


auto const conditional_expression_def = *(logical_or_expression >> '?' >> expression >> ':') >> logical_or_expression;

auto const logical_or_expression_def = logical_and_expression >> *(x3::lit("||") >> logical_and_expression);
auto const logical_and_expression_def = inclusive_or_expression >> *("&&" >> inclusive_or_expression);
auto const inclusive_or_expression_def = exclusive_or_expression >> *('|' >> exclusive_or_expression);
auto const exclusive_or_expression_def = and_expression >> *('^' >> and_expression);
auto const and_expression_def = equality_expression >> *('&' >> equality_expression);
auto const equality_expression_def = relational_expression >> *(equality_expression_oper >> relational_expression);
auto const relational_expression_def = shift_expression >> *(relational_oper >> shift_expression);
auto const shift_expression_def = additive_expression >> *(shift_oper >> additive_expression);

auto const additive_expression_def = multiplicative_expression >> *(additive_oper >> multiplicative_expression);
auto const multiplicative_expression_def = cast_expression >> *(multiplicative_oper >> cast_expression);
auto const cast_expression_def = *('(' >> type_name >> ')') >> unary_expression;

auto const unary_expression_def = *unary_expression_oper >> ((unary_operator >> cast_expression) | (x3::lit("sizeof") >> type_name) | postfix_expression);
// auto const unary_expression_def = *unary_expression_oper >> (unary_operator >> cast_expression);

// custom change
//auto const postfix_expression_def = primary_expression >> *(('[' >> expression >> ']') | ('(' >> *assignment_expression >> ')') | ('.' >>  identifier) | ("->" >>  identifier) | "++" | "--" );
auto const postfix_expression_def = primary_expression >> *(('[' >> expression >> ']') | ('(' >> expression >> ')') | ('.' >>  identifier) | ("->" >>  identifier) | "++" | "--" );


//FIXME: missing enumeration-constant
auto const primary_expression_def = x3::lit('(') >> expression >> ')'
| constant
| (x3::lexeme['\'' >> (x3::char_ - '\'') >> '\''])
| literal_string
| identifier;

//FIXME:  integer-constant> | <character-constant> | <floating-constant> | <enumeration-constant>
// auto const constant_def = (x3::lexeme['\'' >> (x3::char_ - '\'') >> '\''])
// | x3::float_ | x3::double_
// | x3::int_ | x3::long_
// ;


auto const constant_def = x3::real_parser<double, x3::strict_real_policies<double>>{} //
| x3::int_parser<intmax_t>{};
;



auto const expression_def = assignment_expression % ',';
auto const assignment_expression_def = *(unary_expression >> assignment_operator) >> conditional_expression;
auto const assignment_operator_def = x3::string("*=") | x3::string("/=") | x3::string("%=") | x3::string("+=") | x3::string("-=") | x3::string("<<=") | x3::string(">>=") | x3::string("&=") | x3::string("^=") | x3::string("|=") | x3::string("=");
auto const unary_operator_def = x3::char_('&') | x3::char_('*') | x3::char_('+') | x3::char_('-') | x3::char_('~') | x3::char_('!');
auto const type_name_def = +specifier_qualifier >> -abstract_declarator;
auto const parameter_type_list_def = parameter_list
| (parameter_list >> ',' >> "...");
auto const parameter_list_def = parameter_declaration % ',';
auto const parameter_declaration_def = +declaration_specifier >> -(declarator | abstract_declarator);
auto const abstract_declarator_def = pointer
| (pointer >> direct_abstract_declarator)
| direct_abstract_declarator;

//auto const direct_abstract_declarator_def = (x3::lit('(') >> abstract_declarator >> ')') >>  *((x3::lit('[') >> conditional_expression  >> x3::lit(']')) | (x3::lit('(') >> parameter_type_list  >> x3::lit(')')) | (x3::lit('(') >> ')') | (x3::lit('[') >> ']'));
auto const direct_abstract_declarator_def = (x3::lit('(') >> abstract_declarator >> ')') >> *((x3::lit('[') >> -conditional_expression  >> x3::lit(']')) | (x3::lit('(') >> -parameter_type_list  >> x3::lit(')')));



auto const enum_specifier_def = x3::lit("enum") >> (( identifier >> '{' >> enumerator_list >> '}') | ('{' >> enumerator_list >> '}') |  identifier);
auto const enumerator_list_def = enumerator % ',';
auto const enumerator_def = ( identifier >> '=' >> conditional_expression)
|  identifier;

auto const typedef_name_def =  identifier;
auto const declaration_def = +declaration_specifier >> *init_declarator >> ';';

auto const init_declarator_def = ((declarator >> '=' >> initializer) | declarator);
auto const initializer_def = ('{' >> ( (initializer_list >> ',') | initializer_list ) >> '}')
| assignment_expression;

auto const initializer_list_def = initializer % ',';
auto const compound_statement_def = x3::lit('{') >> *declaration >> *statement >> '}';
auto const statement_def = labeled_statement
| expression_statement
| compound_statement
| selection_statement
| iteration_statement
| jump_statement;
auto const labeled_statement_def = ( identifier >> ':' >>  statement)
| (x3::lit("case") >> conditional_expression >> ':' >> statement)
| (x3::lit("default") >> ':' >> statement);
auto const expression_statement_def = -expression >> ";";
auto const selection_statement_def = (x3::lit("if") >> '(' >> expression >> ')' >> (( statement >> "else" >> statement) | statement))
| (x3::lit("switch") >> '(' >> expression >> ')' >> statement);

auto const iteration_statement_def = (x3::lit("while") >> '(' >> expression >> ')' >> statement)
| (x3::lit("do") >> statement >> "while" >> '(' >> expression >> ')' >> ';')
| (x3::lit("for") >> '(' >> -expression >> ';' >> -expression >> ';' >> -expression >> ')' >> statement);




//auto const jump_statement_def = ((x3::lit("goto") >>  identifier) | "continue" | "break" | "return" | ("return" >> expression) ) >> ';';
auto const jump_statement_def = (x3::lit("goto") >>  identifier >> ';')
| (x3::lit("continue")  >> ';')
| (x3::lit("break") >> ';')
| (x3::lit("return") >> -expression >> ';');

//BOOST_SPIRIT_DEFINE(translation_unit,external_declaration,function_definition,declaration_specifier/*,storage_class_specifier*/,type_specifier);
BOOST_SPIRIT_DEFINE(translation_unit);
BOOST_SPIRIT_DEFINE(external_declaration);
BOOST_SPIRIT_DEFINE(function_definition);
BOOST_SPIRIT_DEFINE(declaration_specifier);
BOOST_SPIRIT_DEFINE(type_specifier);


BOOST_SPIRIT_DEFINE(struct_or_union_specifier/*,struct_or_union*/,struct_declaration,specifier_qualifier,struct_declarator_list,struct_declarator,declarator,pointer);
BOOST_SPIRIT_DEFINE(/*type_qualifier,*/direct_declarator,conditional_expression,logical_or_expression,logical_and_expression);
BOOST_SPIRIT_DEFINE(inclusive_or_expression,exclusive_or_expression,and_expression,equality_expression,relational_expression,shift_expression);
BOOST_SPIRIT_DEFINE(additive_expression,multiplicative_expression,cast_expression,unary_expression,postfix_expression,primary_expression,constant);
BOOST_SPIRIT_DEFINE(expression,assignment_expression/*,assignment_operator,unary_operator*/,type_name,parameter_type_list,parameter_list,parameter_declaration);
BOOST_SPIRIT_DEFINE(abstract_declarator,direct_abstract_declarator,enum_specifier,enumerator_list,enumerator,typedef_name,declaration,init_declarator);
BOOST_SPIRIT_DEFINE(initializer,initializer_list,compound_statement,statement,labeled_statement,expression_statement,selection_statement,iteration_statement,jump_statement);

}


#endif /* rule_def_h */
