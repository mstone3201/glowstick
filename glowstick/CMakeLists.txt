# Dependencies
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Source files
add_library(glowstick STATIC
    src/error.cpp
    src/renderer.cpp
    src/vulkan/context.cpp
    src/vulkan/error.cpp
    src/vulkan/device.cpp
    src/vulkan/queue.cpp
)

target_compile_definitions(glowstick PRIVATE $<$<CONFIG:Debug>:GLOWSTICK_DEBUG>)

target_compile_options(glowstick PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

# Link and include
target_link_libraries(glowstick PRIVATE Vulkan::Vulkan glfw glm::glm)
target_include_directories(glowstick PRIVATE src PUBLIC include)
