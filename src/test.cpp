#include "Engine.hpp"

int main(){
    CbLib::VkEngine::Engine engine = CbLib::VkEngine::Engine();

    while(!engine.get_window().should_close()){
        glfwPollEvents();
    }

    engine.cleanup();

    return 0;
}
