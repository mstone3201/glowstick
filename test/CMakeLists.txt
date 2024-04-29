# Source files
add_executable(glowstick_test src/main.cpp)

# Link and include
target_link_libraries(glowstick_test PRIVATE glowstick)
