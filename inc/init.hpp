#ifndef _INIT
#define _INIT
#include <vector>
#include <vulkan/vulkan_core.h>


//Initialise the entire instance
void Initialisation(
    VkInstance *Instance,std::vector<VkPhysicalDevice> *DeviceList, 
    VkCommandPool *CmdPool);


//Initialise a buffer and assign it to a pool.
void CreateBuffer(
    VkCommandBuffer *Buffer,
    VkCommandBufferAllocateInfo *Cmd,
    VkCommandPool CmdPool, 
    VkCommandBufferLevel Level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);





#endif