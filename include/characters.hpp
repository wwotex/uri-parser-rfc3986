#pragma once

#include <array>
#include <string_view>

#define CHARS_SCHEME_NON_ALNUM          ".+-"
#define CHARS_SUB_DELIMS                "!$&'()*+,;="
#define CHARS_UNRESERVED_NON_ALNUM      "-._~"
#define CHARS_REGNAME                   CHARS_UNRESERVED_NON_ALNUM CHARS_SUB_DELIMS
#define CHARS_PCHAR                     CHARS_UNRESERVED_NON_ALNUM CHARS_SUB_DELIMS ":@"
#define CHARS_QUERY_FRAGMENT            CHARS_PCHAR "/?"


constexpr std::array<bool, 256> get_char_lookup_table(std::string_view characters) {
    std::array<bool, 256> table = {};
    for (const char &c: characters) {
        table[c] = true;
    }
    return table;
}
