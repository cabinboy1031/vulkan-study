#ifndef __WINDOW_HPP_
#define __WINDOW_HPP_

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>

#include "IVkObject.hpp"

namespace CbLib::VkEngine{
    class Window:public IVkObject{
        public:
            Window() {};
            Window(uint32_t, uint32_t);
            void destruct();

            bool should_close();
        private:
            GLFWwindow *window;

    };
}


#endif // __WINDOW_HPP_
