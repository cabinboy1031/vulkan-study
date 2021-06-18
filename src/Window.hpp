#ifndef __WINDOW_HPP_
#define __WINDOW_HPP_
#include "Engine.hpp"

namespace CbLib::VkEngine{
    class Window{
        public:
            Window(uint32_t, uint32_t);
            ~Window();

            bool should_close();
        private:
            GLFWwindow *window;

    };
}


#endif // __WINDOW_HPP_
