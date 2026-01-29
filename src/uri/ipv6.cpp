#include <iostream>

#include "uri/characters.hpp"
#include "uri/parse_error.hpp"
#include "uri/uri.h"

bool URI::try_consume_h16() {
    if (m_curr > m_uri.size() || !std::isxdigit(m_uri[m_curr])) {
        return false;
    }
    m_curr++;

    for (int i = 0; i < 3; i++) {
        if (m_curr > m_uri.size() || !std::isxdigit(m_uri[m_curr])) {
            return true;
        }
        m_curr++;
    }

    return true;
}

void URI::consume_ipv6() {
    const std::size_t start = m_curr;

    int seen_double_colon = 0;
    int left = 8;
    bool can_be_number = true;
    bool can_be_colon = true;
    while (m_curr < m_uri.size() && (left - seen_double_colon) > 0) {
        if (can_be_colon && try_consume_char(':')) {
            if (!seen_double_colon && try_consume_char(':')) {
                seen_double_colon = 1;
            } else if (can_be_number || (m_curr < m_uri.size() && !std::isxdigit(m_uri[m_curr]))) {
                throw ParseError("Single colon at the start or end of an IPv6 address not allowed!", m_curr);
            }

            can_be_colon = false;
            can_be_number = true;
            continue;
        }

        if (can_be_number && left >= 2 && try_consume_ipv4()) {
            m_ipv4_address = "";
            left -= 2;
            break;
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

    m_ipv6_address = std::string_view(m_uri.data() + start, m_curr - start);
}

bool URI::try_consume_ipvfuture() {
    const std::size_t start = m_curr;

    if (!try_consume_char('v')) {
        return false;
    }

    if (m_curr >= m_uri.size() || !std::isxdigit(m_uri[m_curr])) {
        return false;
    }

    m_curr++;

    if (!try_consume_char('.')) {
        m_curr = start;
        return false;
    }

    if (!try_consume_generic(get_char_lookup_table(CHARS_IPV_FUTURE), false)) {
        m_curr = start;
        return false;
    }

    while (try_consume_generic(get_char_lookup_table(CHARS_IPV_FUTURE), false));

    m_ipvfuture_address = std::string_view(m_uri.data() + start, m_curr - start);
    return true;
}

bool URI::try_consume_ipv6_vfuture() {
    if (!try_consume_char('[')) {
        return false;
    }

    if (!try_consume_ipvfuture()) {
        consume_ipv6();
    }

    if (!try_consume_char(']')) {
        m_ipv6_address = "";
        m_ipvfuture_address = "";
        throw ParseError("Expected closing square bracket after ipv6 or ipvfuture address!", m_curr);
    }

    return true;
}
