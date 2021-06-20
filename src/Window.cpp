#include "Window.hpp"

using namespace CbLib::VkEngine;

Window::Window(uint32_t width, uint32_t height){
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);


    window = glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);
}

bool Window::should_close(){
    return glfwWindowShouldClose(window);
}

void Window::destruct(){
    glfwDestroyWindow(window);
    glfwTerminate();
}
