#include "vulkan.h"

//#define GLM_FORCE_RADIANS
// #define CGLM_FORCE_DEPTH_ZERO_TO_ONE
// #include <cglm/mat4.h>
// #include <cglm/vec4.h>

int mainLoop(GLFWwindow* window);

int main() {
    int status = 0;

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE, NULL, NULL);

    VkInstance* instance = initVulkanInstance();
    if (instance == NULL) {
        return EXIT_FAILURE;
    }

    VkPhysicalDevice* physicalDevice =  initPhysicalDevice(instance);
    if (physicalDevice == NULL) {
        return EXIT_FAILURE;
    }

    VkDevice* device =  createLogicalDevice(physicalDevice);
    if (device == NULL) {
        return EXIT_FAILURE;
    }

    status = mainLoop(window);
    if (status != 0) {
        return status;
    }

    // Cleanup
    vkDestroyDevice(*device, NULL);
    vkDestroyInstance(*instance, NULL);

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}

int mainLoop(GLFWwindow* window) {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    return 0;
}
