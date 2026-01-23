#include <cassert>
#include <cstdint>
#include <format>
#include <functional>
#include <iostream>
#include <boost/url/parse.hpp>

#define CHARS_SCHEME_NON_ALNUM          ".+-"
#define CHARS_SUB_DELIMS                "!$&'()*+,;="
#define CHARS_UNRESERVED_NON_ALNUM      "-._~"
#define CHARS_PCHAR                     CHARS_UNRESERVED_NON_ALNUM CHARS_SUB_DELIMS ":@"
#define CHARS_QUERY_FRAGMENT            CHARS_PCHAR "/?"

constexpr std::array<bool, 256> get_char_lookup_table(std::string_view characters) {
    std::array<bool, 256> table = {};
    for (const char &c: characters) {
        table[c] = true;
    }
    return table;
}

template<typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v) {
    os << "[";
    for (size_t i = 0; i < v.size(); ++i) {
        os << "\"" << v[i] << "\"";
        if (i != v.size() - 1) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

class ParseError : public std::runtime_error {
public:
    size_t position;

    ParseError(const std::string &message, size_t pos)
        : std::runtime_error(message + " at position " + std::to_string(pos) + "!\n"), position(pos) {
    }
};

class URI {
    size_t m_curr = 0;

    void consume_or_throw(const std::function<bool(char)> &is_char_valid, const std::string &error_msg) {
        if (m_curr >= uri.size() || !is_char_valid(uri[m_curr])) {
            throw ParseError(error_msg, m_curr);
        }

        m_curr++;
    }

    bool try_consume_char(const char required) {
        if (uri[m_curr] != required) {
            return false;
        }

        m_curr++;
        return true;
    }


    bool try_consume_double_slash() {
        if (m_curr + 1 >= uri.size() || uri[m_curr] != '/' || uri[m_curr + 1] != '/') {
            return false;
        }

        m_curr += 2;
        return true;
    }

    void parse_scheme() {
        size_t start = m_curr;

        consume_or_throw([](char tested_char) { return std::isalpha(tested_char); }, "Required alpha character");

        auto is_valid_scheme_char = [](const char &tested) {
            return std::isalnum(tested) || get_char_lookup_table(CHARS_SCHEME_NON_ALNUM)[tested];
        };

        while (m_curr < uri.size() && is_valid_scheme_char(uri[m_curr])) {
            m_curr++;
        }

        scheme = std::string_view(uri.data() + start, m_curr - start);
    }

    void parse_authority() {
        std::cout << "UNIMPLEMENTED PARSING AUTHORITY" << std::endl; //TODO: UNIMPLEMENTED
        // [ userinfo "@" ] host [ ":" port ]
    }

    bool try_consume_char_from_set(const std::array<bool, 256> &lookup) {
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

    void consume_path() {
        size_t path_start = m_curr;
        size_t segment_start = m_curr;
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

    void consume_query_or_fragment(bool is_query) {
        const char separator = is_query ? '?' : '#';
        if (m_curr >= uri.size() || !try_consume_char(separator)) {
            return;
        }

        size_t entity_start = m_curr;

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

public:
    const std::string uri;
    bool has_error = false;
    std::string_view scheme;
    std::string_view encoded_path;
    std::vector<std::string_view> encoded_segments;
    std::string_view encoded_query;
    std::string_view encoded_fragment;

    URI(const std::string_view uri) : uri{uri} {
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
};

void test_uri(std::string_view test_case) {
    std::printf("\t'%.*s'\n", static_cast<int>(test_case.size()), test_case.data());

    URI uri = URI(test_case);
    std::printf("full: \t\t\t%s (%s)\n", uri.uri.data(), uri.has_error ? "ERROR" : "VALID");
    if (uri.scheme.data()) {
        std::cout << "scheme: \t\t" << uri.scheme << "\n";
    }

    if (uri.encoded_path.data()) {
        std::cout << "enc_path: \t\t" << uri.encoded_path << "\n";
        std::cout << "enc_segments: \t" << uri.encoded_segments << "\n";
    }

    if (uri.encoded_query.data()) {
        std::cout << "enc_query: \t\t" << uri.encoded_query << "\n";
    }

    if (uri.encoded_fragment.data()) {
        std::cout << "enc_fragment: \t" << uri.encoded_fragment << "\n";
    }

    std::printf("\n");
}

int main() {
    std::printf("\n\n");
    // test_uri("h@@@");
    // test_uri("h@@@:");
    // test_uri("1abc:");
    // test_uri("");
    // test_uri("a");
    // test_uri(":");
    // test_uri("mambo:");
    // test_uri("mambo:/");
    // test_uri("https://www.google.com/a/b/c/d");
    // test_uri("https:/a/b/c/d");
    // test_uri("https:mambo/italiano/");
    // test_uri("https:mambo/chacha");
    // test_uri("https:/samba/bossa");
    test_uri("https:/www/abc/sdf/er/?mambo=123&123x=23");
    test_uri("https:/www/abc/sdf/er?mambo=makaren%65123#xxxxx/xx/x//x?????");
    test_uri("https:/www/abc/sdf/er?mambo=makaren%65123#xxxxx/xx/x//x?????#####");


    // auto r = boost::urls::parse_uri("https://mamBo/p%40th/to/res").value();
    // std::cout << "host: " << r.host() << "\n";

    return 0;
}
