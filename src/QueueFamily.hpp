#ifndef __QUEUEFAMILYINDICIES_H_
#define __QUEUEFAMILYINDICIES_H_

#include <vector>
#include <optional>

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>

namespace CbLib::VkEngine{
    class QueueFamily{
        public:
            QueueFamily() {}
            QueueFamily(VkPhysicalDevice device);

            bool is_complete();
            std::optional<uint32_t> getGraphicsFamily() {return indices.graphicsFamily; }

        private:
            struct QueueFamilyIndices{
                std::optional<uint32_t> graphicsFamily;
            } indices;
    };

}

#endif // __QUEUEFAMILY_H_
