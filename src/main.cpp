// Google: What are some of the things you wouldnt wish on your worst enemy?
// Lemmie tell you mine.
// I could wish the worst to my enemies... but NEVER to code the vulkan API from scratch without documentation.

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <optional>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../inc/init.hpp"

int main() {

    VulkanInit VkEngine;

    try {
        VkEngine.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
VkResult createDebugUtilsMessengerEXT(
    VkInstance Instance, 
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger); 