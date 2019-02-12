// Google: What are some of the things you wouldnt wish on your worst enemy?
// Lemmie tell you mine.
// I could wish the worst to my enemies... but NEVER to code the vulkan API from scratch without documentation.
#define OUT

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <vector>

#include <vulkan/vulkan.h>

void InitInstance(VkInstanceCreateInfo* InstInfo, std::vector<const char*> InstanceExtensionNames);
void InitQueue();
void InitBuffer();

VkResult InitScreen(VkInstance Instance, VkSurfaceKHR Surface, uint32_t Width, uint32_t Height);

void InitSwapchain();


int main() {
}
