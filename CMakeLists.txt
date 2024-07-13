cmake_minimum_required(VERSION 3.25)
project(shad_1)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE DEBUG)

add_compile_options(-fsanitize=address,undefined)
add_link_options(-fsanitize=address,undefined)

add_executable(shad_1 coins.cpp)
