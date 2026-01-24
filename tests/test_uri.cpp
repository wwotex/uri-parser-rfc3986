#include <cassert>
#include <iostream>

#include "test_ipv6.hpp"
#include "uri/uri.h"

void test_scheme() {
    {
        const auto uri = URI("");
        assert(uri.has_error);
        assert(uri.scheme == "");
        assert(uri.encoded_path == "");
    }
    {
        const auto uri = URI("1abc:");
        assert(uri.has_error);
        assert(uri.scheme == "");
        assert(uri.encoded_path == "");
    }
    {
        const auto uri = URI("abc");
        assert(uri.has_error);
        assert(uri.scheme == "abc");
        assert(uri.encoded_path == "");
    }
    {
        const auto uri = URI("a:");
        assert(!uri.has_error);
        assert(uri.scheme == "a");
        assert(uri.encoded_path == "");
    }
    {
        const auto uri = URI("a123+123.abc-x:/");
        assert(!uri.has_error);
        assert(uri.scheme == "a123+123.abc-x");
        assert(uri.encoded_path == "/");
    }
}

void test_authority() {
    {
        const auto uri = URI("https://git@mambo.com:123/path");
        assert(!uri.has_error);
        assert(uri.scheme == "https");
        assert(uri.encoded_authority == "git@mambo.com:123");
        assert(uri.encoded_userinfo == "git");
        assert(uri.encoded_reg_name == "mambo.com");
        assert(uri.port == "123");
        assert(uri.encoded_path == "/path");
    }
}

int main() {
    test_scheme();
    test_authority();
    test_ipv6();

    std::cout << "All tests PASSED! \n\n";
}
