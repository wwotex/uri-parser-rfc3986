#include <cstdio>
#include <cstdio>
#include <iostream>
#include <string_view>

#include "print_helper.hpp"
#include "../include/uri/uri.h"

inline void test_uri(std::string_view test_case) {
    std::printf("\t'%.*s'\n", static_cast<int>(test_case.size()), test_case.data());

    URI uri = URI(test_case);
    std::printf("full: \t\t\t%s (%s)\n", uri.uri.data(), uri.has_error ? "ERROR" : "VALID");

    std::cout << "scheme: \t\t" << uri.scheme << "\n";

    std::cout << "enc_authority:\t" << uri.encoded_authority << "\n";
    std::cout << "enc_userinfo: \t" << uri.encoded_userinfo << "\n";
    std::cout << "enc_reg_name: \t" << uri.encoded_reg_name << "\n";
    std::cout << "ipv4_address: \t" << uri.ipv4_address << "\n";
    std::cout << "ipv6_address: \t" << uri.ipv6_address << "\n";
    std::cout << "ipvfuture:    \t" << uri.ipvfuture_address << "\n";
    std::cout << "port:         \t" << uri.port << "\n";
    std::cout << "path:         \t" << uri.encoded_path << "\n";

    // if (uri.encoded_path.data()) {
    //     std::cout << "enc_path: \t\t" << uri.encoded_path << "\n";
    //     std::cout << "enc_segments: \t" << to_str(uri.encoded_segments) << "\n";
    // }
    //
    // if (uri.encoded_query.data()) {
    //     std::cout << "enc_query: \t\t" << uri.encoded_query << "\n";
    // }
    //
    // if (uri.encoded_fragment.data()) {
    //     std::cout << "enc_fragment: \t" << uri.encoded_fragment << "\n";
    // }

    std::printf("\n");
}


int main() {
    std::printf("\n\n");
    // test_uri("https://www.google.com/a/b/c/d");
    // test_uri("https:/a/b/c/d");
    // test_uri("https:mambo/italiano/");
    // test_uri("https:mambo/chacha");
    // test_uri("https:/samba/bossa");
    // test_uri("https:/www/abc/sdf/er/?mambo=123&123x=23");
    // test_uri("https:/www/abc/sdf/er?mambo=makaren%65123#xxxxx/xx/x//x?????");
    // test_uri("https:/www/abc/sdf/er?mambo=makaren%65123#xxxxx/xx/x//x?????#####");
    // test_uri("https://123.0.99.255here_is_a_funny_domain_name.com/this/is/the/path");

    // no host
    // test_uri("https://git@:22/this/is/a/path");


    // test_uri("https://git@mambo.com:123");
    // test_uri("https://git@mambo.com:123/PATH/a/b/c?123#5");
    //
    // test_uri("https://10.4.1.255:9999/path");
    // test_uri("https://10.4.1.255:/path");
    // test_uri("https://abc@10.4.1.255:/path");
    // test_uri("https://abc@regname:/path");
    // test_uri("https://www.google.com:/path");
    // test_uri("https://www.google.com/path");

    test_uri("https://[2001:cb0:0:0:fc0::abc]/path");
    test_uri("https://git@[::]:1883/path");
    test_uri("https://git@[1:2:3:4:5:6:7:8]:/path");
    test_uri("https://git@[0001:002:03:4::]/path");
    test_uri("https://[::abc:123:04fc]:443/path");
    test_uri("https://[::2:3:4:5:6:7:8]:443/path");

    // // should fail triple colon
    // test_uri("https://[:::]/path");
    // test_uri("https://[12:abc:dd:::1234]/path");

    // // should fail 2 double colons
    // test_uri("https://[abc::123:6::04fc]:443/path");
    //
    // // should fail too many numbers
    // test_uri("https://[1:2:3:4:5:6:7::8]:443/path");
    // test_uri("https://[1:2:3:4:5:6:7:8::]:443/path");
    // test_uri("https://[::1:2:3:4:5:6:7:8]:443/path");
    //
    // // should fail too few numbers
    // test_uri("https://[1:2:3]:443/path");
    // test_uri("https://[1:2:3:4:5:6:7]:443/path");
    // test_uri("https://git@[1:2:3:4:5:6:7]/path");

    // // should fail no closing bracket
    // test_uri("https://git@[::");
    // test_uri("https://git@[:::1234");
    // test_uri("https://git@[::/path");
}
