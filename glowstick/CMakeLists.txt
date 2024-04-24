# Source files
add_library(glowstick SHARED src/glowstick.cpp)

# Dependencies
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Link and include
target_link_libraries(glowstick PRIVATE Vulkan::Vulkan glfw glm::glm)
target_include_directories(glowstick PUBLIC include)
