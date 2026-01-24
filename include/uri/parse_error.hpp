#pragma once
#include <stdexcept>
#include <string>

class ParseError : public std::runtime_error {
public:
    std::size_t position;

    ParseError(const std::string &message, std::size_t pos)
        : std::runtime_error(message + " at position " + std::to_string(pos) + "!\n"), position(pos) {
    }
};
