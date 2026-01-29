#include <cassert>
#include <iostream>

#include "test_ipv6.hpp"
#include "uri/uri.h"

void test_scheme() {
    {
        const auto uri = URI("");
        assert(uri.has_error());
        assert(uri.scheme().empty());
        assert(uri.encoded_path().empty());
    }
    {
        const auto uri = URI("1abc:");
        assert(uri.has_error());
        assert(uri.scheme().empty());
        assert(uri.encoded_path().empty());
    }
    {
        const auto uri = URI("abc");
        assert(uri.has_error());
        assert(uri.scheme() == "abc");
        assert(uri.encoded_path().empty());
    }
    {
        const auto uri = URI("a:");
        assert(!uri.has_error());
        assert(uri.scheme() == "a");
        assert(uri.encoded_path().empty());
    }
    {
        const auto uri = URI("a123+123.abc-x:/");
        assert(!uri.has_error());
        assert(uri.scheme() == "a123+123.abc-x");
        assert(uri.encoded_path() == "/");
    }
}

void test_authority() {
    {
        const auto uri = URI("https://git@mambo.com:123/path");
        assert(!uri.has_error());
        assert(uri.scheme() == "https");
        assert(uri.encoded_authority() == "git@mambo.com:123");
        assert(uri.encoded_userinfo() == "git");
        assert(uri.encoded_reg_name() == "mambo.com");
        assert(uri.port() == "123");
        assert(uri.encoded_path() == "/path");
    }
}

void test_path() {
    {
        const auto uri = URI("a:/mambo/ch%40cha/rueda?herei%40m%20#heyyou%21");
        assert(!uri.has_error());
        assert(uri.path() == "/mambo/ch@cha/rueda");
        assert(uri.segments()[0] == "mambo");
        assert(uri.segments()[1] == "ch@cha");
        assert(uri.segments()[2] == "rueda");
        assert(uri.query() == "herei@m ");
        assert(uri.fragment() == "heyyou!");
    }
}

int main() {
    test_path();
    test_scheme();
    test_authority();
    test_ipv6();
    test_ipv6_with_ipv4_ext();
    test_ipvfuture();

    std::cout << "All tests PASSED! \n\n";
}
