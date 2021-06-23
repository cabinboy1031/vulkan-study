#include "QueueFamily.hpp"

using namespace CbLib::VkEngine;


QueueFamily::QueueFamily(VkPhysicalDevice device){
    // assign index to queue families that could be found.
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        if(is_complete()){
            break;
        }

        i++;
    }
}

bool QueueFamily::is_complete(){
    return indices.graphicsFamily.has_value();
}
