#pragma once
#include <ranges>
#include <vector>
#include <sstream>

template<typename T>
std::string el_to_str(T el) {
    std::stringstream ss;
    if constexpr (std::is_same_v<std::remove_cvref_t<T>, std::string>) {
        ss << "\"" << el << "\"";
    } else {
        ss << el;
    }
    return ss.str();
}

template<typename T>
std::string to_str(std::vector<T> vec) {
    std::string result;

    if (vec.empty()) {
        return "[]";
    }

    result += "[" + el_to_str(vec[0]);

    for (const T &el: vec | std::views::drop(1)) {
        result += ", " + el_to_str(el);
    }

    result += "]";
    return result;
}
