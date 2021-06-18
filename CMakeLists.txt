cmake_minimum_required(VERSION 3.7 FATAL_ERROR)
project(vulkan-study VERSION 0.0.1)


find_package(Vulkan REQUIRED)
#find_package(glfw REQUIRED)
add_executable(vulkan-study 
    src/main.cpp
    inc/init.hpp
    src/init.cpp
    )

add_executable(vulkan-engine
  src/test.cpp
  src/Window.cpp)


set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw/)

target_link_libraries(vulkan-study glfw Vulkan::Vulkan )
target_link_libraries(vulkan-engine glfw Vulkan::Vulkan )

set_property(TARGET vulkan-study PROPERTY CXX_STANDARD 17)
set_property(TARGET vulkan-study PROPERTY CXX_STANDARD_REQUIRED ON)
