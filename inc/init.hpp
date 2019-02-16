#ifndef _INIT
#define _INIT

const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 400;

const std::vector<const char*> ValidationLayers = {
    "VK_LAYER_LUNARG_standard_validation"
};

#ifdef NDEBUG
    const bool EnableValidationLayers = false;
#else 
    const bool EnableValidationLayers = true;
#endif

class VulkanInit{
public:
    void run() ;
private:
    GLFWwindow* Window;
    VkInstance Instance;
    VkDebugUtilsMessengerEXT DebugMessenger;

    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();

    //all the vulkan stuff
    void instanceInit();
    std::vector<const char*> getRequiredExtensions();

    //Debug functions
    bool checkValidationLayerSupport();
    void setupDebugMessenger();
    

    VkResult createDebugUtilsMessengerEXT(
        VkInstance Instance, 
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
        const VkAllocationCallbacks* pAllocator,
        VkDebugUtilsMessengerEXT* pDebugMessenger);

    void destroyDebugUtilsMessengerEXT(VkInstance Instance, VkDebugUtilsMessengerEXT DebugMessenger, const VkAllocationCallbacks* pAllocator);

};
#endif