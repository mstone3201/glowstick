#include "glowstick/renderer.hpp"
#include "vulkan/context.hpp"

namespace glowstick {
    struct renderer::impl {
        vulkan::context context;
        std::vector<vulkan::device> devices;
    };

    renderer::renderer() :
        p_impl(std::make_unique<renderer::impl>())
    {
        p_impl->devices = p_impl->context.find_devices();
    }

    renderer::~renderer() = default;
}
