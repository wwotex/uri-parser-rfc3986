#pragma once
#include "uri/uri.h"

inline void test_ipv6() {
    {
        const auto uri = URI("s://[::]");
        assert(!uri.has_error);
        assert(uri.ipv6_address == "::");
    }
    {
        const auto uri = URI("s://[::1]");
        assert(!uri.has_error);
        assert(uri.ipv6_address == "::1");
    }
    {
        const auto uri = URI("s://[2001:db8::1]");
        assert(!uri.has_error);
        assert(uri.ipv6_address == "2001:db8::1");
    }
    {
        const auto uri = URI("s://[1:2:3:4:5:6:7:8]");
        assert(!uri.has_error);
        assert(uri.ipv6_address == "1:2:3:4:5:6:7:8");
    }
    {
        const auto uri = URI("s://[0001:0002:0003:0004:0005:0006:0007:0008]");
        assert(!uri.has_error);
        assert(uri.ipv6_address == "0001:0002:0003:0004:0005:0006:0007:0008");
    }
    {
        const auto uri = URI("s://[FFFF:abcd:EF01:2345:6789:ABCD:ef01:2345]");
        assert(!uri.has_error);
        assert(uri.ipv6_address == "FFFF:abcd:EF01:2345:6789:ABCD:ef01:2345");
    }
    {
        const auto uri = URI("s://[2001::]");
        assert(!uri.has_error);
        assert(uri.ipv6_address == "2001::");
    }
    {
        const auto uri = URI("s://[::2001]");
        assert(!uri.has_error);
        assert(uri.ipv6_address == "::2001");
    }
    {
        const auto uri = URI("s://[::1]");
        assert(!uri.has_error);
        assert(uri.ipv6_address == "::1");
    }
    {
        const auto uri = URI("s://[2001:db8::1]");
        assert(!uri.has_error);
        assert(uri.ipv6_address == "2001:db8::1");
    }
    {
        const auto uri = URI("s://[1:2:3:4:5:6:7:8]");
        assert(!uri.has_error);
        assert(uri.ipv6_address == "1:2:3:4:5:6:7:8");
    }
    {
        const auto uri = URI("s://[0001:0002:0003:0004:0005:0006:0007:0008]");
        assert(!uri.has_error);
        assert(uri.ipv6_address == "0001:0002:0003:0004:0005:0006:0007:0008");
    }
    {
        const auto uri = URI("s://[FFFF:abcd:EF01:2345:6789:ABCD:ef01:2345]");
        assert(!uri.has_error);
        assert(uri.ipv6_address == "FFFF:abcd:EF01:2345:6789:ABCD:ef01:2345");
    }
    {
        const auto uri = URI("s://[2001::]");
        assert(!uri.has_error);
        assert(uri.ipv6_address == "2001::");
    }
    {
        const auto uri = URI("s://[::2001]");
        assert(!uri.has_error);
        assert(uri.ipv6_address == "::2001");
    }
    {
        // With port
        const auto uri = URI("s://[::1]:8080");
        assert(!uri.has_error);
        assert(uri.ipv6_address == "::1");
    }
    {
        const auto uri = URI("s://[]");
        assert(uri.has_error);
        assert(uri.ipv6_address == "");
    }
    {
        const auto uri = URI("s://[:::]");
        assert(uri.has_error);
        assert(uri.ipv6_address == "");
    }
    {
        const auto uri = URI("s://[2001:db8:::1]");
        assert(uri.has_error);
        assert(uri.ipv6_address == "");
    }
    {
        // Not enough hextets
        const auto uri = URI("s://[1:2:3:4:5:6:7]");
        assert(uri.has_error);
        assert(uri.ipv6_address == "");
    }
    {
        // Too many hextets (nine)
        const auto uri = URI("s://[1:2:3:4:5:6:7:8:9]");
        assert(uri.has_error);
        assert(uri.ipv6_address == "");
    }
    {
        // Missing closing bracket
        const auto uri = URI("s://[::1");
        assert(uri.has_error);
        assert(uri.ipv6_address == "");
    }
    {
        // Missing opening bracket
        const auto uri = URI("s://::1]");
        assert(uri.has_error);
        assert(uri.ipv6_address == "");
    }
    {
        // Invalid hex digit
        const auto uri = URI("s://[::g]");
        assert(uri.has_error);
        assert(uri.ipv6_address == "");
    }
    {
        const auto uri = URI("s://[ ::1 ]");
        assert(uri.has_error);
        assert(uri.ipv6_address == "");
    }
    {
        const auto uri = URI("s://[12345:111:1:1::]");
        assert(uri.has_error);
        assert(uri.ipv6_address == "");
    }
    {
        const auto uri = URI("s://[::gggg]");
        assert(uri.has_error);
        assert(uri.ipv6_address == "");
    }
}
