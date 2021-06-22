#ifndef __INSTANCE_HPP_
#define __INSTANCE_HPP_

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>

#include "ValidationLayers.hpp"
#include "IVkObject.hpp"

namespace CbLib::VkEngine{
    class Instance:public IVkObject {
    public:
            Instance() {};
            Instance(std::string appName,ValidationLayers validationlayers);

            const VkInstance& getInstance() {return instance;}
            void destruct();
    private:
        VkInstance instance;
    };
}

#endif // __INSTANCE_H_
