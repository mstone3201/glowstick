#include "vulkan/queue.hpp"

namespace glowstick::vulkan {
    queue::queue(VkQueue vk_queue, std::uint32_t family_index) :
        vk_queue(vk_queue),
        family_index(family_index)
    {}

    queue::queue(queue&& other) noexcept :
        vk_queue(other.vk_queue),
        family_index(other.family_index)
    {
        other.vk_queue = VK_NULL_HANDLE;
    }

    queue& queue::operator=(queue&& other) noexcept {
        vk_queue = other.vk_queue;
        other.vk_queue = VK_NULL_HANDLE;

        family_index = other.family_index;

        return *this;
    }

    queue::~queue() = default;

    std::uint32_t queue::get_family_index() const noexcept {
        return family_index;
    }
}
