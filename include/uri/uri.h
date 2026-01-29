#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <functional>
#include <optional>

class URI {
    size_t m_curr = 0;
    const std::string m_uri;
    bool m_has_error = false;
    std::string_view m_scheme;
    std::string_view m_encoded_authority;
    std::string_view m_encoded_userinfo;
    std::string m_userinfo;

    std::string_view m_encoded_reg_name;
    std::string m_reg_name;

    std::string_view m_encoded_path;
    std::string m_path;

    std::vector<std::string_view> m_encoded_segments;
    std::vector<std::string> m_segments;

    std::string_view m_encoded_query;
    std::string m_query;

    std::string_view m_encoded_fragment;
    std::string m_fragment;

    std::string_view m_ipv4_address;
    std::string_view m_ipv6_address;
    std::string_view m_ipvfuture_address;
    std::string_view m_port;

    void consume_or_throw(const std::function<bool(char)> &is_char_valid, const std::string &error_msg);

    bool try_consume_char(char required);

    bool try_consume_double_slash();

    void parse_scheme();

    char get_pct_decoded_char(const char *pos) const;

    void consume_authority();

    std::optional<char> try_consume_generic(const std::array<bool, 256> &lookup, bool allow_pct);

    void consume_path();

    void consume_query_or_fragment(bool is_query);

    bool try_consume_dec_octet();

    bool try_consume_ipv4();

    void try_consume_userinfo();

    void consume_reg_name();

    void try_consume_port();

    bool try_consume_h16();

    void consume_ipv6();

    bool try_consume_ipvfuture();

    bool try_consume_ipv6_vfuture();

public:
    explicit URI(std::string_view uri);

    [[nodiscard]] const std::string &original() const {
        return m_uri;
    }

    [[nodiscard]] bool has_error() const {
        return m_has_error;
    }

    [[nodiscard]] std::string_view scheme() const {
        return m_scheme;
    }

    [[nodiscard]] std::string_view encoded_authority() const {
        return m_encoded_authority;
    }

    [[nodiscard]] std::string_view encoded_userinfo() const {
        return m_encoded_userinfo;
    }

    [[nodiscard]] const std::string &userinfo() const {
        return m_userinfo;
    }

    [[nodiscard]] std::string_view encoded_reg_name() const {
        return m_encoded_reg_name;
    }

    [[nodiscard]] const std::string &reg_name() const {
        return m_reg_name;
    }

    [[nodiscard]] std::string_view encoded_path() const {
        return m_encoded_path;
    }

    [[nodiscard]] const std::string &path() const {
        return m_path;
    }

    [[nodiscard]] const std::vector<std::string_view> &encoded_segments() const {
        return m_encoded_segments;
    }

    [[nodiscard]] const std::vector<std::string> &segments() const {
        return m_segments;
    }

    [[nodiscard]] std::string_view encoded_query() const {
        return m_encoded_query;
    }

    [[nodiscard]] const std::string &query() const {
        return m_query;
    }

    [[nodiscard]] std::string_view encoded_fragment() const {
        return m_encoded_fragment;
    }

    [[nodiscard]] const std::string &fragment() const {
        return m_fragment;
    }

    [[nodiscard]] std::string_view ipv4_address() const {
        return m_ipv4_address;
    }

    [[nodiscard]] std::string_view ipv6_address() const {
        return m_ipv6_address;
    }

    [[nodiscard]] std::string_view ipvfuture_address() const {
        return m_ipvfuture_address;
    }

    [[nodiscard]] std::string_view port() const {
        return m_port;
    }
};
