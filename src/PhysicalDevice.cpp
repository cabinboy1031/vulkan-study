#include "PhysicalDevice.hpp"

using namespace CbLib::VkEngine;
//helper function to find a suitable physical device
bool isDeviceSuitable(VkPhysicalDevice device){
    QueueFamily family(device);

    return family.is_complete();
};


PhysicalDevice::PhysicalDevice(Instance& instance){
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance.getInstance(), &deviceCount, nullptr);

    if(deviceCount == 0) {
        throw std::runtime_error("Failed to find GPUs with vulkan support!");
    }
    devices.resize(deviceCount);
    vkEnumeratePhysicalDevices(instance.getInstance(), &deviceCount, devices.data());

    for(const auto& device: devices){
        if(isDeviceSuitable(device)){
            physicalDevice = device;
            break;
        }
    }

    if(physicalDevice == VK_NULL_HANDLE){
        throw std::runtime_error("Failed to find a suitable GPU!");
    }
}
