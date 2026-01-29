#include <iostream>
#include <array>
#include <functional>
#include <string_view>

#include "uri/uri.h"

#include <charconv>
#include <optional>

#include "uri/characters.hpp"
#include "uri/parse_error.hpp"

void URI::consume_or_throw(const std::function<bool(char)> &is_char_valid, const std::string &error_msg) {
    if (m_curr >= m_uri.size() || !is_char_valid(m_uri[m_curr])) {
        throw ParseError(error_msg, m_curr);
    }

    m_curr++;
}

bool URI::try_consume_char(const char required) {
    if (m_uri[m_curr] != required) {
        return false;
    }

    m_curr++;
    return true;
}


bool URI::try_consume_double_slash() {
    if (m_curr + 1 >= m_uri.size() || m_uri[m_curr] != '/' || m_uri[m_curr + 1] != '/') {
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

    while (m_curr < m_uri.size() && is_valid_scheme_char(m_uri[m_curr])) {
        m_curr++;
    }

    m_scheme = std::string_view(m_uri.data() + start, m_curr - start);
}

char URI::get_pct_decoded_char(const char *pos) const {
    char value = 0;
    auto [ptr, ec] = std::from_chars(pos, pos + 2, value, 16);

    // Check if parsing succeeded and consumed exactly 2 chars
    if (ec != std::errc() || ptr != pos + 2) {
        throw ParseError("Failed to parse PCT encoded char", m_curr);
    }

    return value;
}

std::optional<char> URI::try_consume_generic(const std::array<bool, 256> &lookup, const bool allow_pct) {
    if (m_curr >= m_uri.size()) {
        return std::nullopt;
    }

    if (std::isalnum(m_uri[m_curr]) || lookup[m_uri[m_curr]]) {
        m_curr++;
        return m_uri[m_curr - 1];
    }

    // TODO: add PCT-decoded paths
    if (allow_pct && try_consume_char('%')) {
        consume_or_throw([](const char c) { return std::isxdigit(c); }, "Hexadecimal digit required after %");
        consume_or_throw([](const char c) { return std::isxdigit(c); }, "Hexadecimal digit required after %");

        auto res = get_pct_decoded_char(m_uri.data() + m_curr - 2);
        return res;
    }

    return std::nullopt;
}

void URI::consume_path() {
    std::size_t path_start = m_curr;
    std::size_t segment_start = m_curr;
    std::vector<std::string_view> segments;
    std::vector<std::string> decoded_segments;
    std::string temp_decoded_seg;
    std::string temp_decoded_path;

    while (m_curr < m_uri.size()) {
        if (try_consume_char('/')) {
            if (m_curr - segment_start - 1 > 0) {
                segments.emplace_back(m_uri.data() + segment_start, m_curr - segment_start - 1);
                temp_decoded_path += temp_decoded_seg;
                decoded_segments.push_back(std::move(temp_decoded_seg));
            }
            segment_start = m_curr;
            temp_decoded_path.push_back('/');
            continue;
        }

        auto decoded_char = try_consume_generic(get_char_lookup_table(CHARS_PCHAR), true);
        if (decoded_char.has_value()) {
            temp_decoded_seg.push_back(decoded_char.value());
            continue;
        }

        break;
    }

    if (m_curr - segment_start > 0) {
        segments.emplace_back(m_uri.data() + segment_start, m_curr - segment_start);
        temp_decoded_path += temp_decoded_seg;
        decoded_segments.push_back(std::move(temp_decoded_seg));
    }

    m_encoded_path = std::string_view(m_uri.data() + path_start, m_curr - path_start);
    m_path = std::move(temp_decoded_path);

    m_encoded_segments = std::move(segments);
    m_segments = std::move(decoded_segments);
}

void URI::consume_query_or_fragment(bool is_query) {
    const char separator = is_query ? '?' : '#';
    if (m_curr >= m_uri.size() || !try_consume_char(separator)) {
        return;
    }

    std::size_t entity_start = m_curr;

    std::string decoded_string;
    while (m_curr < m_uri.size()) {
        auto decoded_char = try_consume_generic(get_char_lookup_table(CHARS_QUERY_FRAGMENT), true);
        if (!decoded_char.has_value()) {
            break;
        }

        decoded_string.push_back(decoded_char.value());
    }

    std::string_view temp = std::string_view(m_uri.data() + entity_start, m_curr - entity_start);
    if (is_query) {
        m_encoded_query = temp;
        m_query = std::move(decoded_string);
    } else {
        m_encoded_fragment = temp;
        m_fragment = std::move(decoded_string);
    }
}

URI::URI(const std::string_view uri) : m_uri{uri} {
    try {
        parse_scheme();
        if (!try_consume_char(':')) {
            throw ParseError("Required colon character!", m_curr);
        }

        // hier-part
        if (try_consume_double_slash()) {
            consume_authority();
            consume_path();
        } else {
            consume_path();
        }

        // query and fragment
        consume_query_or_fragment(true);
        consume_query_or_fragment(false);

        if (m_curr < uri.size()) {
            throw ParseError("Invalid character after parsing all elements", m_curr);
        }
    } catch (const ParseError &e) {
        std::cout << "Parsing error: " << e.what();
        m_has_error = true;
    }
}
