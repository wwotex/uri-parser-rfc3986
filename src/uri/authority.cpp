#include <iostream>

#include "uri/characters.hpp"
#include "uri/uri.h"

void URI::try_consume_userinfo() {
    const std::size_t start = m_curr;

    while (m_curr < m_uri.size()) {
        if (try_consume_char('@')) {
            encoded_userinfo = std::string_view(m_uri.data() + start, m_curr - start - 1);
            return;
        }

        if (!try_consume_generic(get_char_lookup_table(CHARS_PCHAR), true)) {
            break;
        }
    }

    m_curr = start;
}


void URI::consume_reg_name() {
    const std::size_t start = m_curr;
    while (try_consume_generic(get_char_lookup_table(CHARS_REGNAME), true));

    encoded_reg_name = std::string_view(m_uri.data() + start, m_curr - start);
}

void URI::try_consume_port() {
    if (!try_consume_char(':')) {
        return;
    }

    const std::size_t start = m_curr;

    while (m_curr < m_uri.size() && std::isdigit(m_uri[m_curr])) {
        m_curr++;
    }

    port = std::string_view(m_uri.data() + start, m_curr - start);
}

void URI::consume_authority() {
    const std::size_t start = m_curr;

    // [ userinfo "@" ] host [ ":" port ]
    try_consume_userinfo();
    // TODO: ipv6 and ipvfuture

    try_consume_ipv6_vfuture() || try_consume_ipv4() || (consume_reg_name(), true);

    try_consume_port();

    encoded_authority = std::string_view(m_uri.data() + start, m_curr - start);
}
