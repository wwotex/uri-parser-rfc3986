#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <functional>

class URI {
    size_t m_curr = 0;

public:
    // TODO: these should be private and have getters and methods should also be private

    const std::string uri;
    bool has_error = false;
    std::string_view scheme;
    std::string_view encoded_authority;
    std::string_view encoded_userinfo;
    std::string_view encoded_host;
    std::string_view encoded_reg_name;
    std::string_view ipv4_address;
    std::string_view ipv6_address;
    std::string_view ipvfuture_address;
    std::string_view port;
    std::string_view encoded_path;
    std::vector<std::string_view> encoded_segments;
    std::string_view encoded_query;
    std::string_view encoded_fragment;


    void consume_or_throw(const std::function<bool(char)> &is_char_valid, const std::string &error_msg);

    bool try_consume_char(char required);

    bool try_consume_double_slash();

    void parse_scheme();

    void parse_authority();

    bool try_consume_generic(const std::array<bool, 256> &lookup);

    void consume_path();

    void consume_query_or_fragment(bool is_query);

    bool parse_dec_octet();

    void try_consume_ipv4();

    void try_consume_userinfo();

    void consume_reg_name();

    explicit URI(std::string_view uri);
};
