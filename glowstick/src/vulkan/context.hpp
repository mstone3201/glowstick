#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "vulkan/device.hpp"

namespace glowstick::vulkan {
    class context {
    public:
        context();
        context(const context&) = delete;
        context(context&& other) noexcept;

        context& operator=(const context&) = delete;
        context& operator=(context&& other) noexcept;

        ~context();

        std::vector<device> find_devices() const;
        
    private:
        VkInstance vk_instance;
    #ifdef GLOWSTICK_DEBUG
        PFN_vkCreateDebugUtilsMessengerEXT vk_create_debug_messenger;
        PFN_vkDestroyDebugUtilsMessengerEXT vk_destroy_debug_messenger;
        VkDebugUtilsMessengerEXT vk_debug_messenger;
    #endif
    };
}
