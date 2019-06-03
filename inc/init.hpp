#ifndef _INIT
#define _INIT

const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 400;

const std::vector<const char*> validationLayers = {
    "VK_LAYER_LUNARG_standard_validation"
};

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

    const std::vector<const char*> DeviceExtentions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    struct QueueFamilyIndices;
    struct SwapChainSupportDetails;
    
    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();

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