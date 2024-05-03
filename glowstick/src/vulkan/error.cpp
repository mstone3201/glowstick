#include "vulkan/error.hpp"

namespace glowstick::vulkan {
    error::error(VkResult result, std::string_view message) :
        glowstick::error(message),
        result(result)
    {}

    error::error(const error& other) noexcept :
        glowstick::error(other),
        result(other.result)
    {}

    error& error::operator=(const error& other) noexcept {
        glowstick::error::operator=(other);
        result = other.result;

        return *this;
    }

    VkResult error::code() const noexcept {
        return result;
    }
}
