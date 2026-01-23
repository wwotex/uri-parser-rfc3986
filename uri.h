#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <functional>

#define CHARS_SCHEME_NON_ALNUM          ".+-"
#define CHARS_SUB_DELIMS                "!$&'()*+,;="
#define CHARS_UNRESERVED_NON_ALNUM      "-._~"
#define CHARS_PCHAR                     CHARS_UNRESERVED_NON_ALNUM CHARS_SUB_DELIMS ":@"
#define CHARS_QUERY_FRAGMENT            CHARS_PCHAR "/?"

class URI {
    size_t m_curr = 0;

public:
    const std::string uri;
    bool has_error = false;
    std::string_view scheme;
    std::string_view encoded_path;
    std::vector<std::string_view> encoded_segments;
    std::string_view encoded_query;
    std::string_view encoded_fragment;

    void consume_or_throw(const std::function<bool(char)> &is_char_valid, const std::string &error_msg);

    bool try_consume_char(char required);

    bool try_consume_double_slash();

    void parse_scheme();

    void parse_authority();

    bool try_consume_char_from_set(const std::array<bool, 256> &lookup);

    void consume_path();

    void consume_query_or_fragment(bool is_query);

    bool parse_dec_octet();

    bool parse_ipv4();

    explicit URI(std::string_view uri);
};
