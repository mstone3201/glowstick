#include "vulkan/device.hpp"

#include <vector>
#include <array>

#include "vulkan/error.hpp"

namespace glowstick::vulkan {
    device::device(VkPhysicalDevice vk_physical_device) :
        vk_device(VK_NULL_HANDLE)
    {
        VkResult result;

        // Get device properties

        VkPhysicalDeviceProperties device_properties;
        vkGetPhysicalDeviceProperties(vk_physical_device, &device_properties);

        device_name = device_properties.deviceName;

        // Get queue families

        // The vulkan spec states that implementations must support at least one
        // queue family, so there is no need to do error checking here
        std::uint32_t family_count;
        vkGetPhysicalDeviceQueueFamilyProperties(vk_physical_device,
            &family_count, nullptr);

        std::vector<VkQueueFamilyProperties> family_properties(family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(vk_physical_device,
            &family_count, family_properties.data());

        // Find suitable queues
        // We are looking for 3 queue families
        // graphics & compute & transfer & present
        //     - for ray tracing and present
        //     - priority 1.0
        // compute & transfer
        //     - for asynchronous matrix updates and AS building
        //     - priority 0.5
        // transfer
        //     - for large asynchronous uploads
        //     - priority 0.0
        // We will require that the device has the first family
        // We will only use up to 1 queue from each family

        struct suitable_family {
            float priority;
            VkQueueFlags required_flags;
            VkQueueFlags allowed_flags;
        };

        // graphics: 0
        // compute: 1
        // transfer: 2
        constexpr std::array<suitable_family, 3> suitable_families{
            suitable_family{
                .priority = 1.0f,
                .required_flags = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT
                    | VK_QUEUE_TRANSFER_BIT,
                .allowed_flags = VK_QUEUE_SPARSE_BINDING_BIT
                    | VK_QUEUE_PROTECTED_BIT
            },
            {
                .priority = 0.5f,
                .required_flags = VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT,
                .allowed_flags = VK_QUEUE_SPARSE_BINDING_BIT
                    | VK_QUEUE_PROTECTED_BIT
            },
            {
                .priority = 0.0f,
                .required_flags = VK_QUEUE_TRANSFER_BIT,
                .allowed_flags = VK_QUEUE_SPARSE_BINDING_BIT
                    | VK_QUEUE_PROTECTED_BIT
            }
        };

        std::array<std::optional<std::uint32_t>, 3> family_indices{};

        std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
        for(std::uint32_t family_index = 0;
            family_index < static_cast<std::uint32_t>(family_properties.size());
            ++family_index)
        {
            auto& family_property = family_properties[family_index];

            for(std::size_t suitable_index = 0;
                suitable_index < suitable_families.size(); ++suitable_index)
            {
                auto& family = suitable_families[suitable_index];

                VkQueueFlags disallowed_flags = ~(family.required_flags
                    | family.allowed_flags);

                // Check that the queue has the flags we want
                if(!family_indices[suitable_index]
                    && (family_property.queueFlags & family.required_flags)
                    == family.required_flags
                    && !(family_property.queueFlags & disallowed_flags))
                {
                    family_indices[suitable_index] = family_index;

                    queue_create_infos.push_back({
                        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                        .queueFamilyIndex = family_index,
                        .queueCount = 1,
                        .pQueuePriorities = &family.priority
                    });

                    break;
                }
            }
        }

        // Create the device

        VkDeviceCreateInfo device_create_info{
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .queueCreateInfoCount =
                static_cast<std::uint32_t>(queue_create_infos.size()),
            .pQueueCreateInfos = queue_create_infos.data()
        };

        result = vkCreateDevice(vk_physical_device, &device_create_info,
            nullptr, &vk_device);
        if(result != VK_SUCCESS)
            throw error(result, "failed to create device");

        // Create queues

        std::array<std::optional<queue>*, 3> queues{
            &graphics_queue,
            &compute_queue,
            &transfer_queue
        };

        for(std::size_t queue_index = 0; queue_index < queues.size();
            ++queue_index)
        {
            if(family_indices[queue_index]) {
                std::uint32_t family_index =
                    family_indices[queue_index].value();

                VkQueue vk_queue;
                vkGetDeviceQueue(vk_device, family_index, 0, &vk_queue);

                queues[queue_index]->emplace(vk_queue, family_index);
            }
        }

        if(!graphics_queue)
            throw glowstick::error("failed to find a suitable graphics queue");
    }

    device::device(device&& other) noexcept :
        vk_device(other.vk_device),
        device_name(std::move(other.device_name)),
        graphics_queue(std::move(other.graphics_queue)),
        compute_queue(std::move(other.compute_queue)),
        transfer_queue(std::move(other.transfer_queue))
    {
        other.vk_device = VK_NULL_HANDLE;

        other.graphics_queue.reset();
        other.compute_queue.reset();
        other.transfer_queue.reset();
    }

    device& device::operator=(device&& other) noexcept {
        vk_device = other.vk_device;
        other.vk_device = VK_NULL_HANDLE;

        device_name = std::move(other.device_name);

        graphics_queue = std::move(other.graphics_queue);
        other.graphics_queue.reset();
        compute_queue = std::move(other.compute_queue);
        other.compute_queue.reset();
        transfer_queue = std::move(other.transfer_queue);
        other.transfer_queue.reset();

        return *this;
    }

    device::~device() {
        vkDestroyDevice(vk_device, nullptr);
    }

    std::string_view device::get_name() const noexcept {
        return device_name;
    }
}
