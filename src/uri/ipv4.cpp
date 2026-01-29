#include <iostream>
#include "uri/uri.h"


bool URI::try_consume_dec_octet() {
    int value = 0;
    if (m_curr >= m_uri.size() || !std::isdigit(m_uri[m_curr])) {
        return false;
    }

    value += (m_uri[m_curr++] - '0');

    if (value == 0 || m_curr >= m_uri.size() || !std::isdigit(m_uri[m_curr])) {
        return true;
    }

    value = 10 * value + (m_uri[m_curr++] - '0');

    if (m_curr >= m_uri.size() || value > 25 || !std::isdigit(m_uri[m_curr])) {
        return true;
    }

    value = 10 * value + (m_uri[m_curr++] - '0');

    if (value > 255) {
        return false;
    }

    return true;
}

bool URI::try_consume_ipv4() {
    const std::size_t start = m_curr;

    bool still_possible = try_consume_dec_octet();
    still_possible = still_possible && try_consume_char('.');
    still_possible = still_possible && try_consume_dec_octet();
    still_possible = still_possible && try_consume_char('.');
    still_possible = still_possible && try_consume_dec_octet();
    still_possible = still_possible && try_consume_char('.');
    still_possible = still_possible && try_consume_dec_octet();

    if (still_possible) {
        m_ipv4_address = std::string_view(m_uri.data() + start, m_curr - start);
    } else {
        m_curr = start;
    }

    return still_possible;
}
