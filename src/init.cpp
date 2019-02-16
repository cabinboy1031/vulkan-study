#include<iostream>
#include<cassert>
#include<cstdlib>
#include<vector>
#include<cstring>
#include<map>

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
    setupDebugMessenger();
    pickPhysicalDevice();
}

void VulkanInit::mainLoop(){
    while(!glfwWindowShouldClose(Window)){
        glfwPollEvents();
    }
}

void VulkanInit::instanceInit(){
    if(EnableValidationLayers && !checkValidationLayerSupport()){
        throw std::runtime_error("Validation layers requested, but not avalible!");
    }
    
    std::cout << "Beggining instance" << std::endl;
    VkApplicationInfo AppInfo = {};
    AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    AppInfo.pNext = nullptr;
    AppInfo.pApplicationName = "Vulkan Study";
    AppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    AppInfo.pEngineName = "No Engine";
    AppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    AppInfo.apiVersion = VK_API_VERSION_1_0;
    
    auto Extensions = getRequiredExtensions();

    VkInstanceCreateInfo InstInfo = {};
    InstInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    InstInfo.ppEnabledExtensionNames = Extensions.data();
    InstInfo.enabledExtensionCount = static_cast<uint32_t>(Extensions.size());
    InstInfo.enabledLayerCount = 0;

    uint32_t ExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &ExtensionCount, nullptr);
    std::vector<VkExtensionProperties> ExtensionProps(ExtensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr,&ExtensionCount,ExtensionProps.data());

    std::cout << "Extensions:" << std::endl;
    for(const auto& Extension : ExtensionProps){
        std::cout << "\t" << Extension.extensionName << std::endl;
    }

    if (EnableValidationLayers) {
        std::cout << "Debug Mode is on! Enabling validation layers. This may impact performance." << std::endl;
        InstInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers.size());
        InstInfo.ppEnabledLayerNames = ValidationLayers.data();
    }

    VkResult Result = vkCreateInstance(&InstInfo,nullptr,&Instance);
    assert(Result == VK_SUCCESS);

    std::cout << "Success!" << std::endl;
}

void VulkanInit::pickPhysicalDevice(){
    uint32_t DeviceCount = 0;
    vkEnumeratePhysicalDevices(Instance,&DeviceCount,nullptr);
    if (DeviceCount == 0) {
        throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> Devices(DeviceCount);
    vkEnumeratePhysicalDevices(Instance,&DeviceCount,Devices.data());

    std::multimap<int,VkPhysicalDevice> Candidates;

    for(const auto& Device : Devices){
        int Score = rateDeviceSuitability(Device);
        Candidates.insert(std::make_pair(Score,Device));
        }

        if (Candidates.rbegin()->first > 0) {
            PhyicalDevice = Candidates.rbegin()->second;
        } else {
            throw std::runtime_error("Failed to find a suitable GPU!");
        }
    }


int VulkanInit::rateDeviceSuitability(VkPhysicalDevice Device){
    VkPhysicalDeviceProperties DeviceProps;
    vkGetPhysicalDeviceProperties(Device,&DeviceProps);
    int Score = 0;

    //Discrete GPUs have a signifigant performance advantage
    if (DeviceProps.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        Score +=1000;
    }

    //Maximum possible size of textures affects graphics quality
    Score += DeviceProps.limits.maxImageDimension2D;

    return Score;
}

bool VulkanInit::checkValidationLayerSupport(){
    std::cout << "Checking for validation layers..." << std::endl;
    uint32_t LayerCount;
    vkEnumerateInstanceLayerProperties(&LayerCount,nullptr);

    std::vector<VkLayerProperties> AvalibleLayers(LayerCount);
    vkEnumerateInstanceLayerProperties(&LayerCount,AvalibleLayers.data());

    for (const char* LayerName : ValidationLayers){
        bool LayerFound = false;

        for (const auto& LayerProperties : AvalibleLayers){
            if(std::strcmp(LayerName,LayerProperties.layerName) == 0){
                LayerFound = true;
                break;
            }
        }

        if (!LayerFound){
            return false;
        }
    }
    return true;
}

std::vector<const char*> VulkanInit::getRequiredExtensions(){
    uint32_t GLFWExtensionCount = 0;
    const char** GLFWExtensions;
    GLFWExtensions = glfwGetRequiredInstanceExtensions(&GLFWExtensionCount);

    std::vector<const char*> Extensions(GLFWExtensions, GLFWExtensions + GLFWExtensionCount);

    if (EnableValidationLayers) {
        Extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return Extensions;

}

//Destroy all vulkan and glfw components
void VulkanInit::cleanup(){
    std::cout << "Closing session..." << std::endl;
    if (EnableValidationLayers) {
        destroyDebugUtilsMessengerEXT(Instance,DebugMessenger,nullptr);
    }


    vkDestroyInstance(Instance,nullptr);
    glfwDestroyWindow(Window);
    glfwTerminate();
}


//Make a Debug message

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallBack(
        VkDebugUtilsMessageSeverityFlagBitsEXT MessageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT MessageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData){
    
    std::cerr << "Validation Layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

void VulkanInit::setupDebugMessenger() {
    if (!EnableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT MessengerInfo = {};
    MessengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    MessengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    MessengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    MessengerInfo.pfnUserCallback = debugCallBack;
    MessengerInfo.pUserData = nullptr; //optional

    if (createDebugUtilsMessengerEXT(Instance,&MessengerInfo,nullptr,&DebugMessenger) != VK_SUCCESS){
        throw std::runtime_error("Failed to set up debug messenger!");
    }
}

VkResult VulkanInit::createDebugUtilsMessengerEXT(VkInstance Instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator,VkDebugUtilsMessengerEXT* pDebugMessenger){
    auto Func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(Instance, "vkCreateDebugUtilsMessengerEXT");
    if(Func != nullptr) {
        return Func(Instance,pCreateInfo,pAllocator,pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void VulkanInit::destroyDebugUtilsMessengerEXT(VkInstance Instance, VkDebugUtilsMessengerEXT DebugMessenger, const VkAllocationCallbacks* pAllocator){
    auto Func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(Instance, "vkDestroyDebugUtilsMessengerEXT");
    if (Func != nullptr) {
        Func(Instance,DebugMessenger,pAllocator);
    }
}