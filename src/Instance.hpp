#ifndef __INSTANCE_HPP_
#define __INSTANCE_HPP_

#include <string>
#include <stdexcept>

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>

#include "IVkObject.hpp"

namespace CbLib::VkEngine{
    class Instance:public IVkObject {
    public:
            Instance(std::string appName);
            void destruct();
    private:
        VkInstance instance;
    };
}

#endif // __INSTANCE_H_
