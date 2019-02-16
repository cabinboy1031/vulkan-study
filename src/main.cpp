// Google: What are some of the things you wouldnt wish on your worst enemy?
// Lemmie tell you mine.
// I could wish the worst to my enemies... but NEVER to code the vulkan API from scratch without documentation.
#define OUT

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <vector>

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
