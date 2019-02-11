#include <iostream>
#include <cstdlib>
#include <vulkan/vulkan.h>
#include <vector>

int main() {
    //Create a basic Vulkan instance
    VkInstance Instance;
    VkResult InstanceRes;
    VkInstanceCreateInfo instInfo;
    instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instInfo.pNext = NULL;
    instInfo.flags = 0;
    instInfo.pApplicationInfo = NULL;
    instInfo.enabledLayerCount = 0;
    instInfo.ppEnabledLayerNames = NULL;
    instInfo.enabledExtensionCount = 0;
    instInfo.ppEnabledExtensionNames = 0;

    InstanceRes = vkCreateInstance(&instInfo, NULL, &Instance);
    if (InstanceRes == VK_ERROR_INCOMPATIBLE_DRIVER) {
        std::cout << "cannot find a compatible Vulkan ICD\n";
        exit(-1);
    } else if (InstanceRes) {
        std::cout << "unknown error\n";
        exit(-1);
    }

    //Get the number of GPUs avalible
    uint32_t gpu_count = 1;
    VkResult GpuRes = vkEnumeratePhysicalDevices(Instance,&gpu_count, NULL);
    //Allocate space and get the list of devices
    std::vector<VkPhysicalDevice> DeviceList;
    DeviceList.resize(gpu_count);
    GpuRes = vkEnumeratePhysicalDevices(Instance,&gpu_count, DeviceList.data()); 

    return 0;
}