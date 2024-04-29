#pragma once

#include <vulkan/vulkan.h>

#include "glowstick/error.hpp"

namespace glowstick::vulkan {
    class error : public glowstick::error {
    public:
        explicit error(VkResult result, std::string_view message);
        error(const error& other) noexcept;

        error& operator=(const error& other) noexcept;

        VkResult code() const noexcept;
        
    private:
        VkResult result;
    };
}
