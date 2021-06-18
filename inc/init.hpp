#ifndef _INIT
#define _INIT

#include<iostream>
#include<cassert>
#include<cstdlib>
#include<vector>
#include<cstring>
#include<map>
#include<optional>
#include<set>
#include<algorithm>
#include<fstream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define OUT

const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 400;

const std::vector<const char*> validationLayers = {
    "VK_LAYER_LUNARG_standard_validation"
};

#define NDEBUG

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else 
    const bool enableValidationLayers = true;
#endif

class VulkanInit{
public:
    void run();

private:
    GLFWwindow* window;
    VkInstance instance;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkSurfaceKHR surface;
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    VkRenderPass renderPass;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;
    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;


    const std::vector<const char*> DeviceExtentions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    struct QueueFamilyIndices;
    struct SwapChainSupportDetails;
    
    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();

    void drawFrame();

void createSemaphores();
    //all the vulkan stuff
    void instanceInit();
    std::vector<const char*> getRequiredExtensions();

    //find and pick a Physical device    
    void pickPhysicalDevice();
    int  rateDeviceSuitability(VkPhysicalDevice device);

    //Find the queue families.
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    bool isDeviceSuitable(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    //Create a logical device
    void createLogicalDevice();

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    //Setup for fully initializing the swapchain
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& avalibleFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& avaliblePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    void createSwapChain();

    void createImageViews();

    void createGraphicsPipeline();
    VkShaderModule createShaderModule(const std::vector<char>& code);

    void createRenderPass();

    void createFramebuffers();

    void createCommandPool();
    void createCommandBuffers();

    static std::vector<char> readFile(const std::string& filename);
    //Create a window surfdce
    void createSurface();

    //Debug functions
    VkDebugUtilsMessengerEXT debugMessenger;

    bool checkValidationLayerSupport();
    void setupDebugMessenger();
    

    VkResult createDebugUtilsMessengerEXT(
        VkInstance instance, 
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
        const VkAllocationCallbacks* pAllocator,
        VkDebugUtilsMessengerEXT* pDebugMessenger);

    void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

};

#endif
