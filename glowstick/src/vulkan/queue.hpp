#pragma once

#include <cstdint>

#include <vulkan/vulkan.h>

namespace glowstick::vulkan {
    class queue {
    public:
        explicit queue(VkQueue vk_queue, std::uint32_t family_index);
        queue(const queue&) = delete;
        queue(queue&& other) noexcept;

        queue& operator=(const queue&) = delete;
        queue& operator=(queue&& other) noexcept;

        ~queue();

        std::uint32_t get_family_index() const noexcept;

    private:
        VkQueue vk_queue;
        std::uint32_t family_index;
    };
}
