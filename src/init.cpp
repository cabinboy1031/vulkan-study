#include<iostream>
#include<cassert>
#include<cstdlib>
#include<vector>
#include<cstring>
#include<map>
#include<optional>
#include<set>
#include<algorithm>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define OUT 
#include "../inc/init.hpp"


struct VulkanInit::QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};
struct VulkanInit::SwapChainSupportDetails{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};
//Start the program.
void VulkanInit::run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}
//Start the GLFW window
void VulkanInit::initWindow(){
    std::cout << "Starting window..." << std::endl;
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API,GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE,GLFW_FALSE);

    window = glfwCreateWindow(WINDOW_WIDTH,WINDOW_HEIGHT,"Vulkan-Study",nullptr,nullptr);
    assert(window != NULL);
    std::cout << "Success!" << std::endl;
}
//The initial code in vulkan
void VulkanInit::initVulkan(){
    instanceInit();
    setupDebugMessenger();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();
    createSwapChain();
    createImageViews();
    createGraphicsPipeline();
}

//The main logic after init
void VulkanInit::mainLoop(){
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
    }
}
//Initialise the instance
void VulkanInit::instanceInit(){
    if(enableValidationLayers && !checkValidationLayerSupport()){
        throw std::runtime_error("Validation layers requested, but not avalible!");
    }
    
    std::cout << "Beggining instance" << std::endl;
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = nullptr;
    appInfo.pApplicationName = "Vulkan Study";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
    
    auto extensions = getRequiredExtensions();

    VkInstanceCreateInfo instInfo = {};
    instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instInfo.ppEnabledExtensionNames = extensions.data();
    instInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    instInfo.enabledLayerCount = 0;

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensionProps(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr,&extensionCount,extensionProps.data());

    std::cout << "extensions:" << std::endl;
    for(const auto& extension : extensionProps){
        std::cout << "\t" << extension.extensionName << std::endl;
    }

    if (enableValidationLayers) {
        std::cout << "Debug Mode is on! Enabling validation layers. This may impact performance." << std::endl;
        instInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        instInfo.ppEnabledLayerNames = validationLayers.data();
    }

    VkResult Result = vkCreateInstance(&instInfo,nullptr,&instance);
    assert(Result == VK_SUCCESS);

    std::cout << "Success!" << std::endl;
}
//Pick a physical device from the list in the computer
void VulkanInit::pickPhysicalDevice(){
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance,&deviceCount,nullptr);
    if (deviceCount == 0) {
        throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance,&deviceCount,devices.data());

    std::multimap<int,VkPhysicalDevice> candidates;

    for(const auto& device : devices){
        int score = rateDeviceSuitability(device);
        candidates.insert(std::make_pair(score,device));
    }

    if (candidates.rbegin()->first > 0) {
            physicalDevice = candidates.rbegin()->second;
    } else {
            throw std::runtime_error("Failed to find a suitable GPU!");
    }
}
//Checks if the device works.
int VulkanInit::rateDeviceSuitability(VkPhysicalDevice device){
    VkPhysicalDeviceProperties deviceProps;
    vkGetPhysicalDeviceProperties(device,&deviceProps);
    int score = 0;

    //Discrete GPUs have a signifigant performance advantage
    if (deviceProps.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score +=1000;
    }

    //Maximum possible size of textures affects graphics quality
    score += deviceProps.limits.maxImageDimension2D;

    return score;
}
//Finds the needed queueFamilies

VulkanInit::QueueFamilyIndices VulkanInit::findQueueFamilies(VkPhysicalDevice device){
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device,&queueFamilyCount OUT, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device,&queueFamilyCount OUT, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily: queueFamilies) {
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device,i,surface,&presentSupport);

        if(queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }
        
        if(queueFamily.queueCount > 0 && presentSupport){
            indices.presentFamily = i;
        }

        if(indices.isComplete()){
            break;
        }

        i++;
    }
    return indices;
}
//Checks if the device is suitable
bool VulkanInit::isDeviceSuitable(VkPhysicalDevice device) {
    QueueFamilyIndices indices = findQueueFamilies(device);

    bool extensionSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if(extensionSupported){
        SwapChainSupportDetails SwapChainSupport = querySwapChainSupport(device);
        swapChainAdequate = !SwapChainSupport.formats.empty() && !SwapChainSupport.presentModes.empty();
    }

    return indices.isComplete() && extensionSupported && swapChainAdequate;
}
//see if the physical device actually supports the extensions we need it for
bool VulkanInit::checkDeviceExtensionSupport(VkPhysicalDevice device){
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device,nullptr,&extensionCount,nullptr);
    
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device,nullptr,&extensionCount,availableExtensions.data());

    std::set<std::string> requiredExtensions(DeviceExtentions.begin(),DeviceExtentions.end());

    for (const auto& extension : availableExtensions){
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}
//Init the logical device
void VulkanInit::createLogicalDevice(){
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(),indices.presentFamily.value()};

    float queuePriority = 1.0f;
    for(uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }
    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(DeviceExtentions.size());
    createInfo.ppEnabledExtensionNames = DeviceExtentions.data();

    if(enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS){
        throw std::runtime_error("Failed to create logical device!");
    }

    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}

//Check for SwapChain support
VulkanInit::SwapChainSupportDetails VulkanInit::querySwapChainSupport(VkPhysicalDevice device){
        SwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device,surface,&formatCount,nullptr);

        if(formatCount != 0){
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device,surface,&formatCount,details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device,surface,&presentModeCount,nullptr);

        if (presentModeCount != 0){
            vkGetPhysicalDeviceSurfacePresentModesKHR(device,surface,&presentModeCount,details.presentModes.data());
        }

        return details;
    }

//Validation layer checking
bool VulkanInit::checkValidationLayerSupport(){
    std::cout << "Checking for validation layers..." << std::endl;
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount,nullptr);

    std::vector<VkLayerProperties> avalibleLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount,avalibleLayers.data());

    for (const char* layerName : validationLayers){
        bool layerFound = false;

        for (const auto& LayerProperties : avalibleLayers){
            if(std::strcmp(layerName,LayerProperties.layerName) == 0){
                layerFound = true;
                break;
            }
        }

        if (!layerFound){
            return false;
        }
    }
    return true;
} 

//extension checking
std::vector<const char*> VulkanInit::getRequiredExtensions(){
    uint32_t GLFWExtensionCount = 0;
    const char** GLFWExtensions;
    GLFWExtensions = glfwGetRequiredInstanceExtensions(&GLFWExtensionCount);

    std::vector<const char*> extensions(GLFWExtensions, GLFWExtensions + GLFWExtensionCount);

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;

}

//Initialise the surface needed for the swapchain to output images to.
void VulkanInit::createSurface(){
    if (glfwCreateWindowSurface(instance,window,nullptr,&surface) != VK_SUCCESS){
        throw std::runtime_error("failed to create window surface!");
    }
}

VkSurfaceFormatKHR VulkanInit::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& avalibleFormats){
    if (avalibleFormats.size() == 1 && avalibleFormats[0].format == VK_FORMAT_UNDEFINED){
        return {VK_FORMAT_B8G8R8A8_UNORM,VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    }

    for (const auto& avalibleFormat : avalibleFormats){
        if (avalibleFormat.format == VK_FORMAT_B8G8R8A8_UNORM && avalibleFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return avalibleFormat;
        }
    }

    return avalibleFormats[0];
}

VkPresentModeKHR VulkanInit::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& avaliblePresentModes){

    VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

    for(const auto& avaliblePresentMode : avaliblePresentModes){
        if (avaliblePresentMode == VK_PRESENT_MODE_MAILBOX_KHR){
            bestMode = avaliblePresentMode;
        } else if (avaliblePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR){
            bestMode = avaliblePresentMode;
        }
    }

    return bestMode;
}

VkExtent2D VulkanInit::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities){
   if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()){
       return capabilities.currentExtent;
   } else {
        VkExtent2D actualExtent = {WINDOW_WIDTH, WINDOW_HEIGHT};

        actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
        
        return actualExtent;
   }
}

void VulkanInit::createSwapChain(){
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    if(swapChainSupport.capabilities.maxImageCount>0 && imageCount > swapChainSupport.capabilities.maxImageCount){
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }
    
    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(),indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(device,&createInfo,nullptr,&swapChain) != VK_SUCCESS){
        throw std::runtime_error("Failed to create swapchain!");
    }

    vkGetSwapchainImagesKHR(device,swapChain,&imageCount,swapChainImages.data());
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device,swapChain,&imageCount,swapChainImages.data());
    
    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;
}

void VulkanInit::createImageViews(){
    swapChainImageViews.resize(swapChainImages.size());

    for (size_t i = 0; i < swapChainImages.size(); i++){
        VkImageViewCreateInfo createInfo = {};
        createInfo.sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image    = swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format   = swapChainImageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask      = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel    = 0;
        createInfo.subresourceRange.levelCount      = 1;
        createInfo.subresourceRange.baseArrayLayer  = 0;
        createInfo.subresourceRange.layerCount      = 1;

        if(vkCreateImageView(device,&createInfo,nullptr,&swapChainImageViews[i]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create image views!");
        }
    }
}

void VulkanInit::createGraphicsPipeline(){
    
}

//Destroy all vulkan and glfw components
void VulkanInit::cleanup(){
    std::cout << "Closing session..." << std::endl;
    if (enableValidationLayers) {
        destroyDebugUtilsMessengerEXT(instance,debugMessenger,nullptr);
    }

    for(auto imageView : swapChainImageViews) {
        vkDestroyImageView(device,imageView,nullptr);
    }

    vkDestroySwapchainKHR(device,swapChain,nullptr);
    vkDestroyDevice(device, nullptr);
    vkDestroySurfaceKHR(instance,surface,nullptr);
    vkDestroyInstance(instance,nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
}


//Make a Debug message
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallBack(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData){
    
    std::cerr << "Validation Layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

void VulkanInit::setupDebugMessenger() {
    if (!enableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT messengerInfo = {};
    messengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    messengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    messengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    messengerInfo.pfnUserCallback = debugCallBack;
    messengerInfo.pUserData = nullptr; //optional

    if (createDebugUtilsMessengerEXT(instance,&messengerInfo,nullptr,&debugMessenger) != VK_SUCCESS){
        throw std::runtime_error("Failed to set up debug messenger!");
    }
}

VkResult VulkanInit::createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator,VkDebugUtilsMessengerEXT* pDebugMessenger){
    auto Func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if(Func != nullptr) {
        return Func(instance,pCreateInfo,pAllocator,pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void VulkanInit::destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator){
    auto Func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (Func != nullptr) {
        Func(instance,debugMessenger,pAllocator);
    }
}
