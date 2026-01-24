#include "../include/characters.hpp"
#include "../include/uri.h"

void URI::try_consume_userinfo() {
    const std::size_t start = m_curr;

    while (m_curr < uri.size()) {
        if (try_consume_char('@')) {
            encoded_userinfo = std::string_view(uri.data() + start, m_curr - start);
            return;
        }

        if (!try_consume_generic(get_char_lookup_table(CHARS_PCHAR))) {
            break;
        }
    }

    m_curr = start;
}


void URI::consume_reg_name() {
    while (try_consume_generic(get_char_lookup_table(CHARS_REGNAME)));
}

void URI::parse_authority() {
    // [ userinfo "@" ] host [ ":" port ]
    try_consume_userinfo();
    try_consume_ipv4();

    consume_reg_name();
}
