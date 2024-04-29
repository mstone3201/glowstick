#pragma once

#include <vulkan/vulkan.h>

namespace glowstick::vulkan {
    class context {
    public:
        context();
        context(const context&) = delete;

        context& operator=(const context&) = delete;

        ~context();
        
    private:
        VkInstance instance;
    };
}
