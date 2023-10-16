project(cc)
cmake_minimum_required(VERSION 3.22.0)
set(BOOST_ROOT /home/sehe/custom/superboost)
find_package(Boost 1.82.0 REQUIRED)

set(CMAKE_CXX_COMPILER g++-13)
set(CMAKE_CXX_STANDARD 23)
add_executable(cc main.cpp)
target_link_libraries(cc Boost::boost)
