#ifndef __ENGINE_HPP_
#define __ENGINE_HPP_

#include <vector>
#include <cstring>

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>

#include "DestructionQueue.hpp"
#include "Window.hpp"
#include "Instance.hpp"
#include "ValidationLayers.hpp"

namespace CbLib {
    namespace VkEngine {
        class Engine{
            public:
                Engine();

                void cleanup(){
                    destructionQueue.flush();
                }

                Window get_window(){ return window; }

            private:
                DestructionQueue destructionQueue;
                Instance instance;
                Window window;
                ValidationLayers validationLayers;

        };
    }
}


#endif // __ENGINE_H_
