cmake_minimum_required(VERSION 3.7 FATAL_ERROR)
project(vulkan-study VERSION 0.0.1)

find_package(Vulkan REQUIRED)
add_executable(vulkan-study 
    src/main.cpp
    )

target_link_libraries(vulkan-study Vulkan::Vulkan)