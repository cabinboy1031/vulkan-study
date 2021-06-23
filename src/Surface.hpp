#ifndef __VULKAN_SURFACE_H_
#define __VULKAN_SURFACE_H_

#include <stdexcept>

#define VK_USE_PLATFORM_XLIB_KHR
#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>

#include "Window.hpp"
#include "Instance.hpp"
#include "IVkObject.hpp"

namespace CbLib::VkEngine{
    class Surface: public IVkObject{
        public:
            Surface() {}
            Surface(Instance vkInstance, Window glfwWindow);

            void destruct() {vkDestroySurfaceKHR(instance, surface, nullptr);}
        private:
            VkSurfaceKHR surface;

            // DO NOT MODIFY THIS AFTER CREATION
            // required for destruction at the end of the objects lifetime
            VkInstance instance;
    };
}


#endif // __SURFACE_H_
