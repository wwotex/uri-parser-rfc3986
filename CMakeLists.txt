cmake_minimum_required(VERSION 4.1)
project(uri_parser)

set(CMAKE_CXX_STANDARD 23)

add_executable(uri_parser main.cpp)

find_package(Boost 1.81.0 REQUIRED COMPONENTS url)
target_link_libraries(uri_parser PRIVATE Boost::url)
