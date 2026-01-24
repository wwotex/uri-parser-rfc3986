#pragma once
#include "uri/uri.h"

inline void test_ipv6() {
    {
        const auto uri = URI("s://[1::2:]");
        assert(uri.has_error);
        assert(uri.ipv6_address == "");
    }
    {
        const auto uri = URI("s://[2001:db8:]");
        assert(uri.has_error);
        assert(uri.ipv6_address == "");
    }
    {
        const auto uri = URI("s://[1::2:");
        assert(uri.has_error);
        assert(uri.ipv6_address == "");
    }
    {
        const auto uri = URI("s://[:12::1]");
        assert(uri.has_error);
        assert(uri.ipv6_address == "");
    }
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
        assert(uri.ipvfuture_address == "");
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

inline void test_ipv6_with_ipv4_ext() {
    {
        const auto uri = URI("s://[::ffff:192.168.0.1]");
        assert(!uri.has_error);
        assert(uri.ipv6_address == "::ffff:192.168.0.1");
    }
    {
        const auto uri = URI("s://[2001:db8::192.0.2.33]");
        assert(!uri.has_error);
        assert(uri.ipv6_address == "2001:db8::192.0.2.33");
    }
    {
        const auto uri = URI("s://[64:ff9b::203.0.113.5]");
        assert(!uri.has_error);
        assert(uri.ipv6_address == "64:ff9b::203.0.113.5");
    }
    {
        const auto uri = URI("s://[2001:db8:abcd:1234::10.1.2.3]");
        assert(!uri.has_error);
        assert(uri.ipv6_address == "2001:db8:abcd:1234::10.1.2.3");
    }
    {
        const auto uri = URI("s://[::ffff:255.255.255.255]");
        assert(!uri.has_error);
        assert(uri.ipv6_address == "::ffff:255.255.255.255");
    }
    // too long
    {
        const auto uri = URI("s://[1:2:3:4:5:6:7:1.2.3.4]");
        assert(uri.has_error);
        assert(uri.ipv6_address == "");
    }
}

inline void test_ipvfuture() {
    {
        // IPvFuture literal: version v1 with "fe80"
        const auto uri = URI("https://[v1.fe80]/path");
        assert(!uri.has_error);
        assert(uri.ipv6_address == "");
        assert(uri.ipvfuture_address == "v1.fe80");
    }
    {
        // IPvFuture with colon in address part and a port
        const auto uri = URI("https://[v2.alpha-1:beta]:443/path");
        assert(!uri.has_error);
        assert(uri.ipv6_address == "");
        assert(uri.ipvfuture_address == "v2.alpha-1:beta");
    }
    {
        // IPvFuture with sub-delims (+ and -) and userinfo
        const auto uri = URI("https://git@[vF.f00-bar+baz]/resource");
        assert(!uri.has_error);
        assert(uri.ipv6_address == "");
        assert(uri.ipvfuture_address == "vF.f00-bar+baz");
    }
}
