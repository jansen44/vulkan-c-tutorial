//#define GLM_FORCE_RADIANS
//#define CGLM_FORCE_DEPTH_ZERO_TO_ONE
//#include <cglm/mat4.h>
//#include <cglm/vec4.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define WIN_TITLE "Vulkan Test: Hello triangle"

struct VKExtensions {
    VkExtensionProperties* extensions;
    uint32_t count;
};

struct GLFWExtensions {
    const char** extensions;
    uint32_t count;
};

struct VKExtensions listAvailableVKExtensions(void);
struct GLFWExtensions listGLFWExtensions(void);
int checkForMissingNecessaryExtensions(struct VKExtensions, struct GLFWExtensions);


VkInstance* initVulkan(void);
int mainLoop(GLFWwindow* window);
int cleanup(GLFWwindow* window);

int main() {
    int status = 0;
    
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    
    GLFWwindow* window =  glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE, NULL, NULL);
    
    VkInstance* instance = initVulkan();
    if (instance == NULL) {
        return EXIT_FAILURE;
    }
    
    status = mainLoop(window);
    if (status != 0) {
        return status;
    }
    
    status = cleanup(window);
    if (status != 0) {
        return status;
    }
    
    // Cleanup
    vkDestroyInstance(*instance, NULL);
    
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return EXIT_SUCCESS;
}

struct VKExtensions listAvailableVKExtensions() {
    // List and print available VK extensions
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);
    
    VkExtensionProperties* extensions = malloc(sizeof(VkExtensionProperties) * extensionCount);
    vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, extensions);
    
    struct VKExtensions exts = {
        .count = extensionCount,
        .extensions = extensions,
    };
    
    return exts;
}

struct GLFWExtensions listGLFWExtensions() {
    uint32_t extensionCount = 0;
    const char** extensions = glfwGetRequiredInstanceExtensions(&extensionCount);
    
    struct GLFWExtensions exts = {
        .count = extensionCount,
        .extensions = extensions,
    };
    
    return exts;
}

int checkForMissingNecessaryExtensions(struct VKExtensions extensions, struct GLFWExtensions glfwExtensions) {
    int found = 0;
    int missingExtensions = 0;

    for (int i = 0; i < glfwExtensions.count; i++) {
        for (int j = 0; j < extensions.count; j++) {
            if (strcmp(glfwExtensions.extensions[i], extensions.extensions[j].extensionName) == 0) {
                found = 1;
                break;
            }
        }
        if (found == 0) {
            fprintf(stderr, "[ERROR] Missing necessary VK extension: %s\n", glfwExtensions.extensions[i]);
            missingExtensions++;
        }

        found = 0;
    }
    
    return missingExtensions;
}


VkInstance* initVulkan() {
    struct VKExtensions extensions = listAvailableVKExtensions();
    
    printf("[INFO] available extensions:\n");
    for(int i = 0; i < extensions.count; i++) {
        printf("[INFO]\t\t%s\n", extensions.extensions[i].extensionName);
    }
    
    // Load GLFW necessary extensions
    struct GLFWExtensions glfwExtensions = listGLFWExtensions();
    
    printf("[INFO] glfw necessary extensions:\n");
    for(int i = 0; i < glfwExtensions.count; i++) {
        printf("[INFO]\t\t%s\n", glfwExtensions.extensions[i]);
    }
    
    int missingExtensionCount = checkForMissingNecessaryExtensions(extensions, glfwExtensions);
    if (missingExtensionCount != 0) {
        fprintf(stderr, "[ERROR] At least %d necessary necessary extensions missing\n", missingExtensionCount);
        return NULL;
    }

    // <<OPTIONAL>> Information about this application that can be useful to the driver in order
    // to optimize this specific application
    VkApplicationInfo appinfo = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = WIN_TITLE,
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "No Engine",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_0,
    };
    
    // This tells which global extensions and validation layers to be used
    VkInstanceCreateInfo createinfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &appinfo,
        .enabledExtensionCount = glfwExtensions.count,
        .ppEnabledExtensionNames = glfwExtensions.extensions,
        .enabledLayerCount = 0,
    };
    
    // Create Instance
    VkInstance* instance = malloc(sizeof(VkInstance));
    VkResult result = vkCreateInstance(&createinfo, NULL, instance);
    if (result != VK_SUCCESS) {
        fprintf(stderr, "[ERROR] Failed to create vk_instance: code %d", result);
        return NULL;
    }
    
    return instance;
}

int mainLoop(GLFWwindow* window) {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
    
    return 0;
}

int cleanup(GLFWwindow* window) {

    
    return 0;
}
