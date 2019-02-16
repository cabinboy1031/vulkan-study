#ifndef _INIT
#define _INIT

const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 400;

class VulkanInit{
public:
    void run() ;
private:
    GLFWwindow* Window;
    VkInstance Instance;

    void initWindow();
    void initVulkan();
    void mainLoop();
    void cleanup();

    //all the vulkan stuff
    void instanceInit();

};
#endif