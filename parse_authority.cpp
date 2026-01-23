#include <iostream>
#include <string_view>
#include "uri.h"


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

bool URI::parse_ipv4() {
    std::cout << "\t" << uri.data() << "\n\n";
    m_curr = 0;

    std::size_t start = m_curr;

    bool still_possible = parse_dec_octet();
    still_possible = still_possible && try_consume_char('.');
    still_possible = still_possible && parse_dec_octet();
    still_possible = still_possible && try_consume_char('.');
    still_possible = still_possible && parse_dec_octet();
    still_possible = still_possible && try_consume_char('.');
    still_possible = still_possible && parse_dec_octet();

    if (!still_possible) {
        m_curr = start;
    }

    std::cout << (still_possible ? "SUCCESS" : "FAILED") << "          curr: " << m_curr << " el: " << (
        (m_curr < uri.size()) ? uri[m_curr] : 'X') << std::endl;
    return still_possible;
}

int main() {
    std::printf("Parsing authority!\n\n");
    URI("abcd").parse_ipv4();
    URI("0.0.0.0").parse_ipv4();
    URI("123.10.0.1").parse_ipv4();
    URI("013.10.0.1").parse_ipv4();
    URI("123.10.256.1").parse_ipv4();
    URI("255.255.255.255").parse_ipv4();
    URI("255.255.255.255abcde").parse_ipv4();
    URI("255:255.255.255abcde").parse_ipv4();
}
