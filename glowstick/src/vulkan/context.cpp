#include "vulkan/context.hpp"

#include <vector>
#include <cstdint>

#include "glowstick/error.hpp"
#include "vulkan/error.hpp"

namespace glowstick::vulkan {
    context::context() : instance(VK_NULL_HANDLE) {
        VkResult result;

        VkApplicationInfo app_info{
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pApplicationName = "glowstick_application",
            .pEngineName = "glowstick",
            .apiVersion = VK_API_VERSION_1_3
        };

        std::vector<const char*> layer_names;

    #ifdef GLOWSTICK_DEBUG
        layer_names.push_back("VK_LAYER_KHRONOS_validation");
    #endif

        VkInstanceCreateInfo create_info{
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pApplicationInfo = &app_info,
            .enabledLayerCount = static_cast<std::uint32_t>(layer_names.size()),
            .ppEnabledLayerNames = layer_names.data()
        };
        
        result = vkCreateInstance(&create_info, nullptr, &instance);
        if(result != VK_SUCCESS)
            throw error(result, "failed to create vulkan instance");
    }

    context::~context() {
        vkDestroyInstance(instance, nullptr);
    }
}
