#include "glowstick/renderer.hpp"
#include "vulkan/context.hpp"

namespace glowstick {
    struct renderer::impl {
        vulkan::context context;
    };

    renderer::renderer() :
        p_impl(std::make_unique<renderer::impl>())
    {}

    renderer::~renderer() = default;
}
