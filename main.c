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

    struct VKSetup* vulkanSetup = initVulkan();
    if (vulkanSetup == NULL) {
        return EXIT_FAILURE;
    }

    status = mainLoop(window);
    if (status != 0) {
        return status;
    }

    // Cleanup
    cleanVulkan(*vulkanSetup);

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
