#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <numeric>
#include <fstream>

#define BOOST_SPIRIT_DEBUG 1
//#define BOOST_SPIRIT_X3_DEBUG 0

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/fusion/include/io.hpp>
#include "rule_def.hpp"

//////////
std::string lua_src_content = R"(
int main(int argc){
    static const int * const const volatile *****a = 3.333;
    return 0;
}
)";

namespace parse {
iterator_type g_iter = lua_src_content.begin();
iterator_type g_end = lua_src_content.end();
}



int main(int argc,const char *argv[]) {    
    
    ast::translation_unit out;
    bool r = parse::x3::phrase_parse(parse::g_iter, parse::g_end, parse::main_rule(), parse::x3::ascii::space, out);
    if(r && (parse::g_iter == parse::g_end)){
        std::cout << "parse success!" << std::endl;
    }else{
        std::cerr << "parse fail!" << std::endl;
    }
    

    return 0;
}
