#include "Instance.hpp"
#include "Window.hpp"

int main(){
    CbLib::VkEngine::Window window(800, 600);

    while(!window.should_close()){
        glfwPollEvents();
    }

    return 0;
}
