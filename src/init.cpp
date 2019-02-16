#include<iostream>
#include<cassert>
#include<cstdlib>
#include<vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../inc/init.hpp"

void VulkanInit::run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

void VulkanInit::initWindow(){
    std::cout << "Starting window..." << std::endl;
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API,GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE,GLFW_FALSE);

    Window = glfwCreateWindow(WINDOW_WIDTH,WINDOW_HEIGHT,"Vulkan-Study",nullptr,nullptr);
    assert(Window != NULL);
    std::cout << "Success!" << std::endl;
}

void VulkanInit::initVulkan(){
    instanceInit();
}

void VulkanInit::mainLoop(){
    while(!glfwWindowShouldClose(Window)){
        glfwPollEvents();
    }
}

void VulkanInit::cleanup(){
    std::cout << "Closing session..." << std::endl;
    glfwDestroyWindow(Window);
    vkDestroyInstance(Instance,nullptr);
    glfwTerminate();
}

void VulkanInit::instanceInit(){
    std::cout << "Beggining instance" << std::endl;
    VkApplicationInfo AppInfo = {};
    AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    AppInfo.pNext = nullptr;
    AppInfo.pApplicationName = "Vulkan Study";
    AppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    AppInfo.pEngineName = "No Engine";
    AppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    AppInfo.apiVersion = VK_API_VERSION_1_0;
    
    uint32_t GLFWExtensionCount = 0;
    const char** GLFWExtensions;

    GLFWExtensions = glfwGetRequiredInstanceExtensions(&GLFWExtensionCount);

    VkInstanceCreateInfo InstInfo = {};
    InstInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    InstInfo.ppEnabledExtensionNames = GLFWExtensions;
    InstInfo.enabledExtensionCount = GLFWExtensionCount;
    InstInfo.enabledLayerCount = 0;

    VkResult Result = vkCreateInstance(&InstInfo,nullptr,&Instance);
    assert(Result == VK_SUCCESS);

    uint32_t ExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &ExtensionCount, nullptr);
    std::vector<VkExtensionProperties> Extensions(ExtensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr,&ExtensionCount,Extensions.data());

    std::cout << "Extensions:" << std::endl;
    for(const auto& Extension : Extensions){
        std::cout << Extension.extensionName << std::endl;
    }

    std::cout << "Success!" << std::endl;
}