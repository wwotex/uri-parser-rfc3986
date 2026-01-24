#include <cassert>
#include <iostream>
#include "uri.h"

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


int main() {
    test_scheme();

    std::cout << "All tests PASSED! \n\n";
}
