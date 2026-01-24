cmake_minimum_required(VERSION 4.1)
project(uri_parser)

set(CMAKE_CXX_STANDARD 23)

set(URI_SOURCE_FILES
        src/uri/authority.cpp
        src/uri/ipv4.cpp
        src/uri/ipv6.cpp
        src/uri/uri.cpp
)

add_executable(main
        src/main.cpp
        ${URI_SOURCE_FILES}
)

add_executable(test_uri
        tests/test_uri.cpp
        tests/test_ipv6.hpp
        ${URI_SOURCE_FILES}
)

add_executable(test_boost
        tests/test_boost.cpp
)

target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(test_uri PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(test_boost PRIVATE ${CMAKE_SOURCE_DIR}/include)

find_package(Boost 1.81.0 REQUIRED COMPONENTS url)
target_link_libraries(test_boost PRIVATE Boost::url)
