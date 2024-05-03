#include "glowstick/error.hpp"

namespace glowstick {
    error::error(std::string_view message) :
        message(make_shared<std::string>(message))
    {}

    error::error(const error& other) noexcept : message(other.message) {}

    error& error::operator=(const error& other) noexcept {
        message = other.message;

        return *this;
    }

    const char* error::what() const noexcept {
        return message->c_str();
    }
}
