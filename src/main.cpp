#define OUT

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <vector>

#include <xcb/xcb.h>
#include <vulkan/vulkan.h>
//Uncomment your system to include correctly
#include <vulkan/vulkan_xcb.h>
//#include <vulkan/vulkan_win32.h>
//#include <vulkan/vulkan_wayland.h>
//#include <vulkan/vulkan_android.h>


int main() {
    //Load extensions
    std::vector<const char*> InstanceExtensionNames;
    InstanceExtensionNames.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
    InstanceExtensionNames.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
    InstanceExtensionNames.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    //Create a basic Vulkan instance
    VkInstance Instance;
    VkResult Result;
    VkInstanceCreateInfo InstInfo;
    InstInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    InstInfo.pNext = NULL;
    InstInfo.flags = 0;
    InstInfo.pApplicationInfo = NULL;
    InstInfo.enabledLayerCount = 0;
    InstInfo.ppEnabledLayerNames = NULL;
    InstInfo.enabledExtensionCount = InstanceExtensionNames.size();
    InstInfo.ppEnabledExtensionNames = InstanceExtensionNames.data();



    Result = vkCreateInstance(OUT &InstInfo, NULL, OUT &Instance);
    if (Result == VK_ERROR_INCOMPATIBLE_DRIVER) {
        std::cout << "cannot find a compatible Vulkan ICD\n";
        exit(-1);
    } else if (Result) {
        std::cout << "unknown error\n";
        exit(-1);
    }

    //Get the number of GPUs avalible
    uint32_t gpu_count = 1;
    VkResult GpuRes = vkEnumeratePhysicalDevices(Instance,OUT &gpu_count, NULL);
    //Allocate space and get the list of devices
    std::vector<VkPhysicalDevice> DeviceList;
    DeviceList.resize(gpu_count);
    GpuRes = vkEnumeratePhysicalDevices(Instance,OUT &gpu_count, DeviceList.data());


    //Queue info
    VkDeviceQueueCreateInfo QueueInfo = {};

    //Initialise first queuefamily and find the properties.
    std::vector<VkQueueFamilyProperties> QueueProps;
    uint32_t QueueFamilyCount = 1;
    
    vkGetPhysicalDeviceQueueFamilyProperties(DeviceList[0],OUT &QueueFamilyCount,NULL);
    QueueProps.resize(QueueFamilyCount); 
    vkGetPhysicalDeviceQueueFamilyProperties(
      DeviceList[0], 
      OUT &QueueFamilyCount,
      QueueProps.data()
    );
    
    //Look for graphics bit
    bool bitIsFound = false;
    for(unsigned int i = 0;i < QueueFamilyCount; i++){
        if (QueueProps[i].queueFlags OUT & VK_QUEUE_GRAPHICS_BIT) {
            QueueInfo.queueFamilyIndex = i;
            bitIsFound = true;
            break;
        }
    }

    //filling out rest of structure
    float QueuePriorities[1] = {0.0};
    QueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    QueueInfo.pNext = NULL;
    QueueInfo.queueCount = 1;
    QueueInfo.pQueuePriorities = QueuePriorities;

    //Creating the logical device
    VkDeviceCreateInfo DeviceInfo = {};
    DeviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    DeviceInfo.pNext = NULL;
    DeviceInfo.queueCreateInfoCount = 1;
    DeviceInfo.pQueueCreateInfos = OUT &QueueInfo;
    DeviceInfo.enabledLayerCount = 0;
    DeviceInfo.ppEnabledLayerNames = NULL;
    DeviceInfo.enabledExtensionCount = 0;
    DeviceInfo.ppEnabledExtensionNames = 0;
    DeviceInfo.pEnabledFeatures = NULL;

    VkDevice Device;
    Result = vkCreateDevice(DeviceList[0],OUT &DeviceInfo,NULL,OUT &Device);
    assert(Result == VK_SUCCESS);

    //Init a command pool
    VkCommandPool CmdPool;
    //Create a command buffer pool
    VkCommandPoolCreateInfo CmdPoolInfo = {};
    CmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    CmdPoolInfo.pNext = NULL;
    CmdPoolInfo.queueFamilyIndex = QueueInfo.queueFamilyIndex;
    CmdPoolInfo.flags = 0;
    //Result Check and allocation
    Result = vkCreateCommandPool(Device, OUT &CmdPoolInfo, NULL,OUT &CmdPool);
    assert(Result == VK_SUCCESS);

    //create the command buffer from the command pool
    VkCommandBuffer CmdBuffer;
    VkCommandBufferAllocateInfo Cmd = {};
    Cmd.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    Cmd.pNext = NULL;
    Cmd.commandPool = CmdPool;
    Cmd.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    Cmd.commandBufferCount = 1;

    Result = vkAllocateCommandBuffers(Device, OUT &Cmd, OUT &CmdBuffer);

    
    
    VkSurfaceKHR Surface;
    //Iterate over each queue to see if it supports presenting
    VkBool32 *pSupportsPresent = (VkBool32 *)malloc(QueueFamilyCount * sizeof(VkBool32));
    for (uint32_t i = 0; i < QueueFamilyCount; i++) {
        vkGetPhysicalDeviceSurfaceSupportKHR(DeviceList[0], i,Surface,OUT &pSupportsPresent[i]);
    }
    
    //Search for a graphics and a present queue in the array of queue families. Try to find one that supports both
    uint32_t GraphicsQueueFamilyIndex = UINT32_MAX;
    uint32_t PresentQueueFamilyIndex = UINT32_MAX;
    for(uint32_t i = 0; i < QueueFamilyCount; ++i) {
        if(QueueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT!=0) {
            if(GraphicsQueueFamilyIndex ==UINT32_MAX)
                GraphicsQueueFamilyIndex = i;

            if(pSupportsPresent[i] == VK_TRUE) {
                GraphicsQueueFamilyIndex = i;
                PresentQueueFamilyIndex = i;
                break;
            }
        }
    }

    if(PresentQueueFamilyIndex == UINT32_MAX) {
        //if didnt find a queue that supports both graphics and present then
        //find a seperate present queue
        for(size_t i = 0; i < QueueFamilyCount; ++i){
            if (pSupportsPresent[i] == VK_TRUE) {
                PresentQueueFamilyIndex = i;
                break;
            }
        }
    }
    free(pSupportsPresent);

    //#if defined(VK_USE_PLATFORM_XCB_KHR)
    const xcb_setup_t *setup;
    xcb_screen_iterator_t iter;
    int scr;

    xcb_connection_t *Connection = xcb_connect(NULL, &scr);
    if (Connection == NULL || xcb_connection_has_error(Connection)) {
        std::cout << "Unable to make an XCB connection\n";
        exit(-1);
    }

    setup = xcb_get_setup(Connection);
    iter = xcb_setup_roots_iterator(setup);
    while (scr-- > 0) xcb_screen_next(&iter);

    xcb_screen_t *Screen = iter.data;

    uint16_t Width = 400;
    uint16_t Height = 400;

    uint32_t value_mask, value_list[32];

    uint32_t Window = xcb_generate_id(Connection);

    value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    value_list[0] = Screen->black_pixel;
    value_list[1] = XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_EXPOSURE;

    xcb_create_window(Connection, XCB_COPY_FROM_PARENT, Window, Screen->root, 0, 0, Width, Height, 0,
                      XCB_WINDOW_CLASS_INPUT_OUTPUT, Screen->root_visual, value_mask, value_list);

    /* Magic code that will send notification when window is destroyed */
    xcb_intern_atom_cookie_t cookie = xcb_intern_atom(Connection, 1, 12, "WM_PROTOCOLS");
    xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(Connection, cookie, 0);

    xcb_intern_atom_cookie_t cookie2 = xcb_intern_atom(Connection, 0, 16, "WM_DELETE_WINDOW");
    xcb_intern_atom_reply_t *AtomWmDeleteWindow = xcb_intern_atom_reply(Connection, cookie2, 0);

    xcb_change_property(Connection, XCB_PROP_MODE_REPLACE, Window, (*reply).atom, 4, 32, 1,
                        &(*AtomWmDeleteWindow).atom);
    free(reply);

    xcb_map_window(Connection, Window);

    // Force the x/y coordinates to 100,100 results are identical in consecutive
    // runs
    const uint32_t coords[] = {100, 100};

    xcb_configure_window(Connection, Window, XCB_CONFIG_WINDOW_Y, coords);
    xcb_flush(Connection);

    xcb_generic_event_t *e;
    while ((e = xcb_wait_for_event(Connection))) {
        if ((e->response_type & ~0x80) == XCB_EXPOSE) break;
    }
    
    //#endif

    // Construct the surface description:
    #ifdef _WIN32
        VkWin32SurfaceCreateInfoKHR CreateInfo = {};
        CreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        CreateInfo.pNext = NULL;
        CreateInfo.hinstance = Connection;
        CreateInfo.hwnd = Window;
        Result = vkCreateWin32SurfaceKHR(Instance,OUT &CreateInfo, NULL, &Surface);
    #elif defined(__ANDROID__)
        GET_INSTANCE_PROC_ADDR(Instance, CreateAndroidSurfaceKHR);

        VkAndroidSurfaceCreateInfoKHR CreateInfo;
        CreateInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
        CreateInfo.pNext = nullptr;
        CreateInfo.flags = 0;
        CreateInfo.window = AndroidGetApplicationWindow();
        Result = info.fpCreateAndroidSurfaceKHR(Instance,OUT &CreateInfo, nullptr, &Surface);
    #elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
        VkWaylandSurfaceCreateInfoKHR CreateInfo = {};
        CreateInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
        CreateInfo.pNext = NULL;
        CreateInfo.display = info.display;
        CreateSurface = Window;
        Result = vkCreateWaylandSurfaceKHR(Instance,OUT &CreateInfo, NULL, &Surface);
    #else
        VkXcbSurfaceCreateInfoKHR CreateInfo = {};
        CreateInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
        CreateInfo.pNext = NULL;
        CreateInfo.connection = Connection;
        CreateInfo.window = Window;
        Result = vkCreateXcbSurfaceKHR(Instance, OUT &CreateInfo, NULL, &Surface);
    #endif  // _WIN32
        assert(Result == VK_SUCCESS);

        VkSwapchainCreateInfoKHR SwapchainCi = {};
        SwapchainCi.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        SwapchainCi.pNext = NULL;
        SwapchainCi.surface = Surface;

        SwapchainCi.imageFormat = VK_FORMAT_B8G8R8A8_UNORM;

        VkSurfaceCapabilitiesKHR SurfCapabilities;
        Result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(DeviceList[0],Surface,OUT &SurfCapabilities);
        assert(Result == VK_SUCCESS);

        uint32_t PresentModeCount;
        Result = vkGetPhysicalDeviceSurfacePresentModesKHR(DeviceList[0],Surface,OUT &PresentModeCount, NULL);
        assert(Result == VK_SUCCESS);

        VkExtent2D SwapchainExtent;
        // width and height are either both 0xFFFFFFFF, or both not 0xFFFFFFFF.
        if (SurfCapabilities.currentExtent.width == 0xFFFFFFFF) {
            // If the surface size is undefined, the size is set to
            // the size of the images requested.
            SwapchainExtent.width = Width;
            SwapchainExtent.height = Height;
            if (SwapchainExtent.width < SurfCapabilities.minImageExtent.width) {
                SwapchainExtent.width = SurfCapabilities.minImageExtent.width;
            } else if (SwapchainExtent.width > SurfCapabilities.maxImageExtent.width) {
                SwapchainExtent.width = SurfCapabilities.maxImageExtent.width;
            }

            if (SwapchainExtent.height < SurfCapabilities.minImageExtent.height) {
                SwapchainExtent.height = SurfCapabilities.minImageExtent.height;
            } else if (SwapchainExtent.height > SurfCapabilities.maxImageExtent.height) {
                SwapchainExtent.height = SurfCapabilities.maxImageExtent.height;
            }
        } else {
            // If the surface size is defined, the swap chain size must match
            SwapchainExtent = SurfCapabilities.currentExtent;
        }

        // The FIFO present mode is guaranteed by the spec to be supported
        VkPresentModeKHR SwapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;

        // Determine the number of VkImage's to use in the swap chain.
        // We need to acquire only 1 presentable image at at time.
        // Asking for minImageCount images ensures that we can acquire
        // 1 presentable image as long as we present it before attempting
        // to acquire another.
        uint32_t DesiredNumberOfSwapChainImages = SurfCapabilities.minImageCount;

        VkSurfaceTransformFlagBitsKHR PreTransform;
        if (SurfCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
            PreTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        } else {
            PreTransform = SurfCapabilities.currentTransform;
        }

        uint32_t DesiredNumOfSwapChainImages = SurfCapabilities.minImageCount;
        SwapchainCi.minImageCount = DesiredNumOfSwapChainImages;
        SwapchainCi.imageExtent.width = SwapchainExtent.width;
        SwapchainCi.imageExtent.width = SwapchainExtent.height;
        SwapchainCi.preTransform = PreTransform;
        SwapchainCi.presentMode = SwapchainPresentMode; 

        SwapchainCi.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        SwapchainCi.queueFamilyIndexCount = 0;
        SwapchainCi.pQueueFamilyIndices = NULL;
        uint32_t queueFamilyIndices[2] = {
            (uint32_t)GraphicsQueueFamilyIndex,
            (uint32_t)PresentQueueFamilyIndex};
        if (GraphicsQueueFamilyIndex != PresentQueueFamilyIndex) {
            // If the graphics and present queues are from different queue families,
            // we either have to explicitly transfer ownership of images between the
            // queues, or we have to create the swapchain with imageSharingMode
             // as VK_SHARING_MODE_CONCURRENT
            SwapchainCi.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            SwapchainCi.queueFamilyIndexCount = 2;
            SwapchainCi.pQueueFamilyIndices = queueFamilyIndices;
        }

        VkSwapchainKHR SwapChain;
        Result = vkCreateSwapchainKHR(Device,OUT &SwapchainCi,NULL,&SwapChain);
        assert(Result == VK_SUCCESS);
        uint32_t SwapchainImageCount;
        Result = vkGetSwapchainImagesKHR(Device,SwapChain,&SwapchainImageCount,NULL);
        assert(Result==VK_SUCCESS);
        VkImage *SwapchainImages = (VkImage *)malloc(SwapchainImageCount * sizeof(VkImage));
        assert(SwapchainImages);
        Result = vkGetSwapchainImagesKHR(Device, SwapChain, &SwapchainImageCount,SwapchainImages);
        assert(Result == VK_SUCCESS);
        typedef struct _swap_chain_buffers {
            VkImage image;
            VkImageView view;
        } SwapchainBuffer;
        
        std::vector<SwapchainBuffer> Buffers;

        Buffers.resize(SwapchainImageCount);
        for(uint32_t i = 0; i < SwapchainImageCount; i++){
            Buffers[i].image = SwapchainImages[i];
        }
        free(SwapchainImages);


        VkImageCreateInfo ImageInfo = {};
        const VkFormat DepthFormat = VK_FORMAT_D16_UNORM;
        VkFormatProperties Props;
        vkGetPhysicalDeviceFormatProperties(DeviceList[0],DepthFormat,&Props);

        ImageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        ImageInfo.pNext = NULL;
        ImageInfo.imageType = VK_IMAGE_TYPE_2D;
        ImageInfo.format = VK_FORMAT_D16_UNORM;
        ImageInfo.extent.width = Width;
        ImageInfo.extent.height = Height;
        ImageInfo.extent.depth = 1;
        ImageInfo.mipLevels = 1;
        ImageInfo.arrayLayers = 1;
        ImageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        ImageInfo.queueFamilyIndexCount = 0;
        ImageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        ImageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        ImageInfo.pQueueFamilyIndices = NULL;
        ImageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        ImageInfo.flags = 0;

        VkMemoryRequirements MemReqs;

        struct {
            VkFormat format;

            VkImage image;
            VkDeviceMemory mem;
            VkImageView view;
        } Depth;
        
        Depth.format = DepthFormat;

        vkCreateImage(Device,OUT &ImageInfo,NULL,OUT &Depth.image);
        vkGetImageMemoryRequirements(Device,Depth.image,&MemReqs);

        VkMemoryAllocateInfo MemAlloc = {};
        MemAlloc.sType = VK_STRUCTURE_TYPE_EXPORT_MEMORY_ALLOCATE_INFO;
        MemAlloc.pNext = NULL;
        MemAlloc.allocationSize = MemReqs.size;
        MemAlloc.memoryTypeIndex = 0;

        VkImageViewCreateInfo ViewInfo = {};
            ViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            ViewInfo.pNext = NULL;
            ViewInfo.image = VK_NULL_HANDLE;
            ViewInfo.format = DepthFormat;
            ViewInfo.components.r = VK_COMPONENT_SWIZZLE_R;
            ViewInfo.components.g = VK_COMPONENT_SWIZZLE_G;
            ViewInfo.components.b = VK_COMPONENT_SWIZZLE_B;
            ViewInfo.components.a = VK_COMPONENT_SWIZZLE_A;
            ViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
            ViewInfo.subresourceRange.baseMipLevel = 0;
            ViewInfo.subresourceRange.levelCount = 1;
            ViewInfo.subresourceRange.baseArrayLayer = 0;
            ViewInfo.subresourceRange.layerCount = 1;
            ViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            ViewInfo.flags = 0;

        ViewInfo.image = Depth.image;

        vkAllocateMemory(Device,&MemAlloc,NULL, &Depth.mem);
        vkBindImageMemory(Device,Depth.image,Depth.mem,0);

        Result = vkCreateImageView(Device,&ViewInfo,NULL,&Depth.view);

        vkDestroyDevice(Device, NULL);
        vkDestroyInstance(Instance, NULL);
        return 0;
    }