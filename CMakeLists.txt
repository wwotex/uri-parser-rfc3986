cmake_minimum_required(VERSION 4.1)
project(uri_parser)

set(CMAKE_CXX_STANDARD 23)

add_executable(uri_parser
        main.cpp
        uri.cpp
        print_helper.hpp
        parse_error.hpp
        uri.h
)

add_executable(parse_authority
        parse_authority.cpp
        uri.cpp
        print_helper.hpp
        uri.h
)

find_package(Boost 1.81.0 REQUIRED COMPONENTS url)
target_link_libraries(uri_parser PRIVATE Boost::url)
