#include "vulkan/context.hpp"

#ifdef GLOWSTICK_DEBUG
#include <iostream>
#endif
#include <vector>
#include <cstdint>

#include "glowstick/error.hpp"
#include "vulkan/error.hpp"

namespace glowstick::vulkan {
    namespace {
    #ifdef GLOWSTICK_DEBUG
        VKAPI_ATTR VkBool32 VKAPI_CALL debug_messenger_callback(
            VkDebugUtilsMessageSeverityFlagBitsEXT severity,
            VkDebugUtilsMessageTypeFlagsEXT types,
            const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
            void*)
        {
            if(severity <= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
                return VK_FALSE;

            std::vector<std::string_view> triggers;
            switch(severity) {
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
                triggers.push_back("VERBOSE");
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
                triggers.push_back("INFO");
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
                triggers.push_back("WARNING");
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
                triggers.push_back("ERROR");
                break;
            }

            if(types & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT)
                triggers.push_back("GENERAL");
            if(types & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
                triggers.push_back("VALIDATION");
            if(types & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
                triggers.push_back("PERFORMANCE");

            std::cerr << '[';

            if(!triggers.empty()) {
                std::string_view last_trigger = triggers.back();
                triggers.pop_back();

                for(std::string_view trigger : triggers)
                    std::cerr << trigger << '|';

                std::cerr << last_trigger;
            }

            std::cerr << "]: " << callback_data->pMessage << std::endl;

            return VK_FALSE;
        }
    #endif
    }

    context::context() :
        vk_instance(VK_NULL_HANDLE)
    #ifdef GLOWSTICK_DEBUG
        ,
        vk_create_debug_messenger(nullptr),
        vk_destroy_debug_messenger(nullptr),
        vk_debug_messenger(VK_NULL_HANDLE)
    #endif
    {
        VkResult result;

        // Create a vulkan instance

        VkApplicationInfo app_info{
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pApplicationName = "glowstick_application",
            .pEngineName = "glowstick",
            .apiVersion = VK_API_VERSION_1_3
        };

        // Layers and extensions
        std::vector<const char*> layer_names;
        std::vector<const char*> extension_names;

    #ifdef GLOWSTICK_DEBUG
        layer_names.push_back("VK_LAYER_KHRONOS_validation");
        extension_names.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    #endif

        VkInstanceCreateInfo instance_create_info{
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pApplicationInfo = &app_info,
            .enabledLayerCount = static_cast<std::uint32_t>(layer_names.size()),
            .ppEnabledLayerNames = layer_names.data(),
            .enabledExtensionCount =
                static_cast<std::uint32_t>(extension_names.size()),
            .ppEnabledExtensionNames = extension_names.data()
        };
        
        result = vkCreateInstance(&instance_create_info, nullptr, &vk_instance);
        if(result != VK_SUCCESS)
            throw error(result, "failed to create vulkan instance");

    #ifdef GLOWSTICK_DEBUG
        // Load debug utils extension functions

        vk_create_debug_messenger =
            reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
                vkGetInstanceProcAddr(vk_instance,
                    "vkCreateDebugUtilsMessengerEXT"));
        if(vk_create_debug_messenger == nullptr)
            throw glowstick::error(
                "could not load vkCreateDebugUtilsMessengerEXT");

        vk_destroy_debug_messenger =
            reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
                vkGetInstanceProcAddr(vk_instance,
                    "vkDestroyDebugUtilsMessengerEXT"));
        if(vk_destroy_debug_messenger == nullptr)
            throw glowstick::error(
                "could not load vkDestroyDebugUtilsMessengerEXT");

        // Create debugger

        VkDebugUtilsMessengerCreateInfoEXT debug_ext_create_info{
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
                | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            .pfnUserCallback = &debug_messenger_callback
        };

        result = vk_create_debug_messenger(vk_instance, &debug_ext_create_info,
            nullptr, &vk_debug_messenger);
        if(result != VK_SUCCESS)
            throw error(result, "failed to create debug messenger");
    #endif
    }

    context::context(context&& other) noexcept :
        vk_instance(other.vk_instance)
    #ifdef GLOWSTICK_DEBUG
        ,
        vk_create_debug_messenger(other.vk_create_debug_messenger),
        vk_destroy_debug_messenger(other.vk_destroy_debug_messenger),
        vk_debug_messenger(other.vk_debug_messenger)
    #endif
    {
        other.vk_instance = VK_NULL_HANDLE;
    #ifdef GLOWSTICK_DEBUG
        other.vk_create_debug_messenger = nullptr;
        other.vk_destroy_debug_messenger = nullptr;
        other.vk_debug_messenger = VK_NULL_HANDLE;
    #endif
    }

    context& context::operator=(context&& other) noexcept {
        vk_instance = other.vk_instance;
        other.vk_instance = VK_NULL_HANDLE;

    #ifdef GLOWSTICK_DEBUG
        vk_create_debug_messenger = other.vk_create_debug_messenger;
        other.vk_create_debug_messenger = nullptr;

        vk_destroy_debug_messenger = other.vk_destroy_debug_messenger;
        other.vk_destroy_debug_messenger = nullptr;

        vk_debug_messenger = other.vk_debug_messenger;
        other.vk_debug_messenger = VK_NULL_HANDLE;
    #endif

        return *this;
    }

    context::~context() {
    #ifdef GLOWSTICK_DEBUG
        if(vk_destroy_debug_messenger)
            vk_destroy_debug_messenger(vk_instance, vk_debug_messenger,
                nullptr);
    #endif
        vkDestroyInstance(vk_instance, nullptr);
    }

    std::vector<device> context::find_devices() const {
        if(vk_instance == VK_NULL_HANDLE)
            throw glowstick::error("tried to use an empty context");

        VkResult result;

        // Gather all the physical devices

        std::uint32_t device_count;
        result = vkEnumeratePhysicalDevices(vk_instance, &device_count,
            nullptr);
        if(result != VK_SUCCESS)
            throw error(result, "failed to get physical device count");
        if(device_count == 0)
            throw glowstick::error("could not find any vulkan devices");

        std::vector<VkPhysicalDevice> physical_devices(device_count);
        result = vkEnumeratePhysicalDevices(vk_instance, &device_count,
            physical_devices.data());
        if(result != VK_SUCCESS)
            throw error(result, "failed to enumerate physical devices");

        // Try to create suitable devices

        std::vector<device> devices;
        for(auto physical_device : physical_devices) {
            try {
                devices.emplace_back(physical_device);
            } catch(error&) {
                continue;
            } catch(glowstick::error&) {
                continue;
            }
        }

        return devices;
    }
}
