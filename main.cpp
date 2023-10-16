#include <iostream>
#include <vector>

//////////
static std::string c_src_content = R"(
    int main(int argc){
        static const int * const const volatile *****a = 3.333;
        return 0;
    }
)";

// #define BOOST_SPIRIT_X3_DEBUG 1
#include "rule_def.hpp"

int main() {
    ast::translation_unit out;
    parse::iterator_type  f = c_src_content.begin(), l = c_src_content.end();

    bool r = parse::x3::phrase_parse(f, l, parse::main_rule() >> parse::x3::eoi, parse::x3::space, out);
    if (r) {
        std::cout << "parse success!" << std::endl;
    } else {
        std::cerr << "parse fail!" << std::endl;
    }
}
