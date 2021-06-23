#include "Engine.hpp"

using namespace CbLib::VkEngine;

Engine::Engine(){
    if(validationLayers.enabled && !validationLayers.checkSupport()){
        throw std::runtime_error("validation layers requested, but not available!");
    }

    instance = Instance("Vulkan engine", validationLayers);
    destructionQueue.push(instance);

    window = Window(600,800);
    destructionQueue.push(window);

    device = Device(instance);
    destructionQueue.push(device);

}
