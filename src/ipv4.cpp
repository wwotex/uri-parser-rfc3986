#include <iostream>
#include "../include/uri.h"


bool URI::parse_dec_octet() {
    int value = 0;
    if (m_curr >= uri.size() || !std::isdigit(uri[m_curr])) {
        return false;
    }

    value += (uri[m_curr++] - '0');

    if (value == 0 || m_curr >= uri.size() || !std::isdigit(uri[m_curr])) {
        return true;
    }

    value = 10 * value + (uri[m_curr++] - '0');

    if (m_curr >= uri.size() || value > 25 || !std::isdigit(uri[m_curr])) {
        return true;
    }

    value = 10 * value + (uri[m_curr++] - '0');

    if (value > 255) {
        return false;
    }

    return true;
}

void URI::try_consume_ipv4() {
    const std::size_t start = m_curr;

    bool still_possible = parse_dec_octet();
    still_possible = still_possible && try_consume_char('.');
    still_possible = still_possible && parse_dec_octet();
    still_possible = still_possible && try_consume_char('.');
    still_possible = still_possible && parse_dec_octet();
    still_possible = still_possible && try_consume_char('.');
    still_possible = still_possible && parse_dec_octet();

    m_curr = still_possible ? m_curr : start;
}
