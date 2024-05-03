#pragma once

#include <string>
#include <optional>

#include <vulkan/vulkan.h>

#include "vulkan/queue.hpp"

namespace glowstick::vulkan {
    class device {
    public:
        explicit device(VkPhysicalDevice vk_physical_device);
        device(const device&) = delete;
        device(device&& other) noexcept;

        device& operator=(const device&) = delete;
        device& operator=(device&& other) noexcept;

        ~device();

        std::string_view get_name() const noexcept;

    private:
        VkDevice vk_device;
        std::string device_name;
        std::optional<queue> graphics_queue;
        std::optional<queue> compute_queue;
        std::optional<queue> transfer_queue;
    };
}
