# Source files
add_executable(test src/main.cpp)

# Link and include
target_link_libraries(test PRIVATE glowstick)
target_include_directories(test PRIVATE include)
