#include <cstdio>
#include <cstdio>
#include <iostream>
#include <string_view>

#include "print_helper.hpp"
#include "uri.h"

inline void test_uri(std::string_view test_case) {
    std::printf("\t'%.*s'\n", static_cast<int>(test_case.size()), test_case.data());

    URI uri = URI(test_case);
    std::printf("full: \t\t\t%s (%s)\n", uri.uri.data(), uri.has_error ? "ERROR" : "VALID");
    if (uri.scheme.data()) {
        std::cout << "scheme: \t\t" << uri.scheme << "\n";
    }

    if (uri.encoded_path.data()) {
        std::cout << "enc_path: \t\t" << uri.encoded_path << "\n";
        std::cout << "enc_segments: \t" << to_str(uri.encoded_segments) << "\n";
    }

    if (uri.encoded_query.data()) {
        std::cout << "enc_query: \t\t" << uri.encoded_query << "\n";
    }

    if (uri.encoded_fragment.data()) {
        std::cout << "enc_fragment: \t" << uri.encoded_fragment << "\n";
    }

    std::printf("\n");
}


int main() {
    std::printf("\n\n");
    test_uri("h@@@");
    test_uri("h@@@:");
    test_uri("1abc:");
    test_uri("");
    test_uri("a");
    test_uri(":");
    test_uri("mambo:");
    test_uri("mambo:/");
    test_uri("https://www.google.com/a/b/c/d");
    test_uri("https:/a/b/c/d");
    test_uri("https:mambo/italiano/");
    test_uri("https:mambo/chacha");
    test_uri("https:/samba/bossa");
    test_uri("https:/www/abc/sdf/er/?mambo=123&123x=23");
    test_uri("https:/www/abc/sdf/er?mambo=makaren%65123#xxxxx/xx/x//x?????");
    test_uri("https:/www/abc/sdf/er?mambo=makaren%65123#xxxxx/xx/x//x?????#####");
    test_uri("https://123.0.99.255here_is_a_funny_domain_name.com/this/is/the/path");

    // no host
    test_uri("https://git@:22/this/is/a/path");
}
