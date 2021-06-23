#include "Surface.hpp"

using namespace CbLib::VkEngine;

Surface::Surface(Instance vkInstance, Window glfwWindow){
    VkXlibSurfaceCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    createInfo.window = glfwGetX11Window(glfwWindow.getWindow());
    createInfo.dpy = glfwGetX11Display();

    if(vkCreateXlibSurfaceKHR(vkInstance.getInstance(),
                            &createInfo,
                            nullptr,
                            &surface) != VK_SUCCESS){
        throw std::runtime_error("Failed to create window surface!");
    }

    instance = vkInstance.getInstance();
}
