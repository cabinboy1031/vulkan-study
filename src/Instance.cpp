#include "Instance.hpp"

using namespace CbLib::VkEngine;

Instance::Instance(std::string appName, ValidationLayers validationLayers){
    // Application Info
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = appName.c_str();
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // Check for extension compatability
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    std::cout << "available extentions: \n";
    for (const auto& extension: extensions) {
        std::cout << "\t" << extension.extensionName << "\n";
    }

    // Instance create info
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> fullExtensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    fullExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
    fullExtensions.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);


    createInfo.enabledExtensionCount = fullExtensions.size();
    createInfo.ppEnabledExtensionNames = fullExtensions.data();

    if(validationLayers.enabled) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.layers.size());
        createInfo.ppEnabledLayerNames = validationLayers.layers.data();
    }else{
        createInfo.enabledLayerCount = 0;
    }

    // Create the instance
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}

void Instance::destruct(){
    vkDestroyInstance(instance, nullptr);
}
