# Project setup
cmake_minimum_required(VERSION 3.28)

project(glowstick)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Automatically export all symbols with MSVC
# This does not work for global and static variables
# However, inline global and static variables will be exported correctly
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Dependencies
add_subdirectory(glowstick)
add_subdirectory(test)
