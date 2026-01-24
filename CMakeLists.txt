cmake_minimum_required(VERSION 4.1)
project(uri_parser)

set(CMAKE_CXX_STANDARD 23)

add_executable(main
        src/main.cpp
        src/uri.cpp
        src/userinfo.cpp
)

add_executable(test_boost
        tests/test_boost.cpp
)

add_executable(test_uri
        tests/test_uri.cpp
        src/uri.cpp
        src/userinfo.cpp
        src/ipv4.cpp
)

target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(test_boost PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(test_uri PRIVATE ${CMAKE_SOURCE_DIR}/include)

find_package(Boost 1.81.0 REQUIRED COMPONENTS url)
target_link_libraries(test_boost PRIVATE Boost::url)
