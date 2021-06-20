#ifndef __ENGINE_HPP_
#define __ENGINE_HPP_

#define GLFW_INCLUDE_VULKAN
#include<GLFW/glfw3.h>
#include "Window.hpp"
#include "Instance.hpp"

namespace CbLib {
    namespace VkEngine {
        class Engine{
            Instance instance;
            Window window;


        };
    }
}


#endif // __ENGINE_H_
