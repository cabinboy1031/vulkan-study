#ifndef __VULKAN_DEVICE_HPP_
#define __VULKAN_DEVICE_HPP_

#include <stdexcept>
#include <optional>

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>

#include "Instance.hpp"
#include "IVkObject.hpp"
#include "QueueFamily.hpp"

namespace CbLib::VkEngine{
    class PhysicalDevice{
        public:
            PhysicalDevice() {}
            PhysicalDevice(Instance& instance);
        private:
            std::vector<VkPhysicalDevice> devices;
            VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
            QueueFamily qFamily;
    };
}


#endif // __DEVICE_H_
