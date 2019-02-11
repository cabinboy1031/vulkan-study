#include <iostream>
#include <cstdlib>
#include <vulkan/vulkan.h>


int main() {
    VkInstance inst;
    VkResult res;
    VkInstanceCreateInfo instInfo;
    instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instInfo.pNext = NULL;
    instInfo.flags = 0;
    instInfo.pApplicationInfo = NULL;
    instInfo.enabledLayerCount = 0;
    instInfo.ppEnabledLayerNames = NULL;
    instInfo.enabledExtensionCount = 0;
    instInfo.ppEnabledExtensionNames = 0;

    res = vkCreateInstance(&instInfo, NULL, &inst);
    if (res == VK_ERROR_INCOMPATIBLE_DRIVER) {
        std::cout << "cannot find a compatible Vulkan ICD\n";
        exit(-1);
    } else if (res) {
        std::cout << "unknown error\n";
        exit(-1);
    }

    return 0;
}