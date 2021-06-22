#ifndef __VULKAN_DEVICE_HPP_
#define __VULKAN_DEVICE_HPP_

#include <stdexcept>

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>

#include "Instance.hpp"
#include "IVkObject.hpp"

namespace CbLib::VkEngine{
    class Device:public IVkObject{
        public:
            Device() {};
            Device(Instance& instance);

            void destruct() {};
        private:
            std::vector<VkPhysicalDevice> devices;
            VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    };
}


#endif // __DEVICE_H_
