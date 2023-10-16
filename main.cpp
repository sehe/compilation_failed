#include <iostream>
#include <vector>

//////////
// #define BOOST_SPIRIT_X3_DEBUG 1
#include "parser_functions.hpp"

#if 0
    static void test_program() {
        if (parse::tester::test_translation_unit(R"(
                int main(int argc){
                    static const int * const const volatile *****a = 3.333;
                    return 0;
                }
            )")) {
            std::cout << "parse success!" << std::endl;
        } else {
            std::cerr << "parse fail!" << std::endl;
        }
    }
#endif


int main() {
    // test_program();
}
