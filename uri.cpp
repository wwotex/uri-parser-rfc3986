#include <cassert>
#include <iostream>
#include <array>
#include <functional>
#include <string_view>

#include "uri.h"
#include "parse_error.hpp"

constexpr std::array<bool, 256> get_char_lookup_table(std::string_view characters) {
    std::array<bool, 256> table = {};
    for (const char &c: characters) {
        table[c] = true;
    }
    return table;
}

void URI::consume_or_throw(const std::function<bool(char)> &is_char_valid, const std::string &error_msg) {
    if (m_curr >= uri.size() || !is_char_valid(uri[m_curr])) {
        throw ParseError(error_msg, m_curr);
    }

    m_curr++;
}

bool URI::try_consume_char(const char required) {
    if (uri[m_curr] != required) {
        return false;
    }

    m_curr++;
    return true;
}


bool URI::try_consume_double_slash() {
    if (m_curr + 1 >= uri.size() || uri[m_curr] != '/' || uri[m_curr + 1] != '/') {
        return false;
    }

    m_curr += 2;
    return true;
}

void URI::parse_scheme() {
    std::size_t start = m_curr;

    consume_or_throw([](char tested_char) { return std::isalpha(tested_char); }, "Required alpha character");

    auto is_valid_scheme_char = [](const char &tested) {
        return std::isalnum(tested) || get_char_lookup_table(CHARS_SCHEME_NON_ALNUM)[tested];
    };

    while (m_curr < uri.size() && is_valid_scheme_char(uri[m_curr])) {
        m_curr++;
    }

    scheme = std::string_view(uri.data() + start, m_curr - start);
}

void URI::parse_authority() {
    std::cout << "UNIMPLEMENTED PARSING AUTHORITY" << std::endl; //TODO: UNIMPLEMENTED
    // [ userinfo "@" ] host [ ":" port ]
}

bool URI::try_consume_char_from_set(const std::array<bool, 256> &lookup) {
    assert(m_curr < uri.size());

    if (std::isalnum(uri[m_curr]) || lookup[uri[m_curr]]) {
        m_curr++;
        return true;
    }

    // TODO: add PCT-decoded paths
    if (try_consume_char('%')) {
        consume_or_throw([](const char c) { return std::isxdigit(c); }, "Hexadecimal digit required after %");
        consume_or_throw([](const char c) { return std::isxdigit(c); }, "Hexadecimal digit required after %");
        return true;
    }

    return false;
}

void URI::consume_path() {
    std::size_t path_start = m_curr;
    std::size_t segment_start = m_curr;
    std::vector<std::string_view> segments;

    while (m_curr < uri.size()) {
        if (try_consume_char('/')) {
            if (m_curr - segment_start - 1 > 0) {
                segments.emplace_back(uri.data() + segment_start, m_curr - segment_start - 1);
            }
            segment_start = m_curr;
            continue;
        }

        if (try_consume_char_from_set(get_char_lookup_table(CHARS_PCHAR))) {
            continue;
        }

        break;
    }

    if (m_curr - segment_start > 0) {
        segments.emplace_back(uri.data() + segment_start, m_curr - segment_start);
    }

    encoded_path = std::string_view(uri.data() + path_start, m_curr - path_start);
    encoded_segments = std::move(segments);
}

void URI::consume_query_or_fragment(bool is_query) {
    const char separator = is_query ? '?' : '#';
    if (m_curr >= uri.size() || !try_consume_char(separator)) {
        return;
    }

    std::size_t entity_start = m_curr;

    while (m_curr < uri.size()) {
        if (!try_consume_char_from_set(get_char_lookup_table(CHARS_QUERY_FRAGMENT))) {
            break;
        }
    }

    std::string_view temp = std::string_view(uri.data() + entity_start, m_curr - entity_start);
    if (is_query) {
        encoded_query = temp;
    } else {
        encoded_fragment = temp;
    }
}

URI::URI(const std::string_view uri) : uri{uri} {
    try {
        // scheme
        parse_scheme();

        // :
        if (!try_consume_char(':')) {
            throw ParseError("Required colon character!", m_curr);
        }

        // hier-part
        if (try_consume_double_slash()) {
            // authority path-abempty
            parse_authority();
        } else {
            consume_path();
            // path-absolute / path-rootless / path-empty
        }

        consume_query_or_fragment(true);
        consume_query_or_fragment(false);
        if (m_curr < uri.size()) {
            throw ParseError("Invalid character after parsing all elements", m_curr);
        }
    } catch (const ParseError &e) {
        std::cerr << "Parsing error: " << e.what();
        has_error = true;
    }
}
