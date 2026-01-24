#include <iostream>

#include "uri/parse_error.hpp"
#include "uri/uri.h"

bool URI::try_consume_h16() {
    if (m_curr > uri.size() || !std::isxdigit(uri[m_curr])) {
        return false;
    }
    m_curr++;

    for (int i = 0; i < 3; i++) {
        if (m_curr > uri.size() || !std::isxdigit(uri[m_curr])) {
            return true;
        }
        m_curr++;
    }

    return true;
}

bool URI::try_consume_ipv6_vfuture() {
    if (!try_consume_char('[')) {
        return false;
    }

    const std::size_t start = m_curr;
    int seen_double_colon = 0;
    int left = 8;
    bool can_be_number = true;
    bool can_be_colon = true;
    while (m_curr < uri.size() && (left - seen_double_colon) > 0) {
        if (can_be_colon && try_consume_char(':')) {
            if (!seen_double_colon && try_consume_char(':')) {
                seen_double_colon = 1;
            }

            can_be_colon = false;
            can_be_number = true;
            continue;
        }

        if (can_be_number && try_consume_h16()) {
            can_be_colon = true;
            can_be_number = false;
            left--;
            continue;
        }

        break;
    }
    if (left > 0 && !seen_double_colon) {
        throw ParseError("Failed to parse IPv6 address!", m_curr);
    }

    if (!try_consume_char(']')) {
        throw ParseError("Expected closing square bracket after ipv6 or ipvfuture address!", m_curr);
    }

    ipv6_address = std::string_view(uri.data() + start, m_curr - start - 1);

    return true;
}
