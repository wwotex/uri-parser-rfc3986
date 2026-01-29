#include <charconv>
#include <cstdio>
#include <cstdio>
#include <iostream>
#include <string_view>

#include "print_helper.hpp"
#include "../include/uri/uri.h"

inline void test_uri(std::string_view test_case) {
    std::cout << "\n\ttest case: '" << test_case << "'\n\n";
    URI uri = URI(test_case);
    std::cout << "full: \t\t\t" << uri.original() << " (" << (uri.has_error() ? "ERROR" : "VALID") << ")\n";

    std::cout << "scheme: \t\t" << uri.scheme() << "\n";

    std::cout << "encoded authority:\t" << uri.encoded_authority() << "\n";
    std::cout << "userinfo:     \t" << uri.userinfo() << "\n";
    std::cout << "enc_reg_name: \t" << uri.reg_name() << "\n";
    std::cout << "ipv4_address: \t" << uri.ipv4_address() << "\n";
    std::cout << "ipv6_address: \t" << uri.ipv6_address() << "\n";
    std::cout << "ipvfuture:    \t" << uri.ipvfuture_address() << "\n";
    std::cout << "port:         \t" << uri.port() << "\n";
    std::cout << "path:         \t" << uri.path() << "\n";
    std::cout << "segments:     \t" << to_str(uri.segments()) << "\n";
    std::cout << "query:        \t" << uri.query() << "\n";
    std::cout << "fragment:     \t" << uri.fragment() << "\n";

    std::printf("\n");
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <uri>\n";
        return 1;
    }

    test_uri(argv[1]);
}
