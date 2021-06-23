#ifndef __VALIDATIONLAYERS_H_
#define __VALIDATIONLAYERS_H_

#include <vector>
#include <cstring>

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>

namespace CbLib::VkEngine{
    class ValidationLayers{
        public:

        bool checkSupport(){
            uint32_t layerCount;
            vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

            std::vector<VkLayerProperties> availableLayers(layerCount);
            vkEnumerateInstanceLayerProperties(&layerCount,availableLayers.data());

            for(const char* layerName: layers){
                bool layerFound = false;

                for(const auto& layerProperties: availableLayers){
                    if (std::strcmp(layerName,layerProperties.layerName) == 0){
                        layerFound = true;
                        break;
                    }
                }
                if(!layerFound){
                    return false;
                }
            }
            return true;
        }

            #ifndef NDEBUG
            const bool enabled = true;
            #else
            const bool enabled= false;
            #endif
            const std::vector<const char*> layers= {
            "VK_LAYER_KHRONOS_validation",
            };
    };
}


#endif // __VALIDATIONLAYERS_H_
