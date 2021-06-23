#ifndef __VULKAN_LOGICAL_DEVICE_HPP_
#define __VULKAN_LOGICAL_DEVICE_HPP_

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>

#include "IVkObject.hpp"
#include "QueueFamily.hpp"
#include "PhysicalDevice.hpp"

namespace CbLib::VkEngine{
    class Device:public IVkObject{
        public:
            Device() {}
            Device(Instance vkInstance);

            void destruct() override { vkDestroyDevice(device, nullptr); }
        private:
            PhysicalDevice selected_device;
            VkDevice device;
            VkQueue graphicsQueue;
    };
}


#endif // __DEVICE_H_
