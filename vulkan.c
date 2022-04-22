#include "vulkan.h"

static const char* validationLayers[] = {
    "VK_LAYER_KHRONOS_validation",
};
#define VALIDATION_LAYERS_AMOUNT (sizeof(validationLayers) / sizeof(const char*))

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

int checkValidationLayerSupport() {
    uint32_t layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, NULL);

    VkLayerProperties* availableLayers = malloc(sizeof(VkLayerProperties) * layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

    int layerFound = 0;
    for (int i = 0; i < VALIDATION_LAYERS_AMOUNT; i++) {
        for (int j = 0; j < layerCount; j++) {
            if (strcmp(validationLayers[i], availableLayers[j].layerName) == 0) {
                layerFound = 1;
                break;
            }
        }

        if (layerFound == 0) {
            fprintf(stderr, "[ERROR] requested validation layer \"%s\" not available!\n", validationLayers[i]);
            return 1;
        }
        layerFound = 0;
    }

    return 0;
}

VkInstance* initVulkanInstance() {
#ifdef ENABLE_VALIDATION_LAYERS
    int found = 0;
    if ((found = checkValidationLayerSupport()) != 0) {
        fprintf(stderr, "[ERROR] validation layers requested, but not available!\n");
        return NULL;
    }
#endif

    // Load VK available extensions
    struct VKExtensions extensions = listAvailableVKExtensions();

    printf("[INFO] available extensions:\n");
    for (int i = 0; i < extensions.count; i++) {
        printf("[INFO]\t\t%s\n", extensions.extensions[i].extensionName);
    }

    // Load GLFW necessary extensions
    struct GLFWExtensions glfwExtensions = listGLFWExtensions();

    printf("[INFO] glfw necessary extensions:\n");
    for (int i = 0; i < glfwExtensions.count; i++) {
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
    };

#ifdef ENABLE_VALIDATION_LAYERS
    createinfo.enabledLayerCount = VALIDATION_LAYERS_AMOUNT;
    createinfo.ppEnabledLayerNames = validationLayers;
#else
    createinfo.enabledLayerCount = 0;
#endif

    // Create Instance
    VkInstance* instance = malloc(sizeof(VkInstance));
    VkResult result = vkCreateInstance(&createinfo, NULL, instance);
    if (result != VK_SUCCESS) {
        fprintf(stderr, "[ERROR] Failed to create vk_instance: code %d", result);
        return NULL;
    }

    return instance;
}

int isDeviceSuitable(VkPhysicalDevice device) {
    // VkPhysicalDeviceProperties deviceProperties;
    // vkGetPhysicalDeviceProperties(device, &deviceProperties);

    // VkPhysicalDeviceFeatures deviceFeatures;
    // vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    // if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader) {
    //     return 1;
    // }

    // return 0;
    return 1;
}

VkPhysicalDevice* initPhysicalDevice(VkInstance* instance) {
    VkPhysicalDevice* physicalDevice = malloc(sizeof(VkPhysicalDevice));
    physicalDevice = VK_NULL_HANDLE;

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(*instance, &deviceCount, NULL);
    if (deviceCount == 0) {
        fprintf(stderr, "[ERROR] Failed to find GPUs with vulkan support!");
        return NULL;
    }

    VkPhysicalDevice* devices = malloc(sizeof(VkPhysicalDevice) * deviceCount);
    vkEnumeratePhysicalDevices(*instance, &deviceCount, devices);

    for (int i = 0; i < deviceCount; i++) {
        if (isDeviceSuitable(devices[i])) {
            physicalDevice = &devices[i];
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE) {
        fprintf(stderr, "[ERROR] Failed to find suitable GPU!");
        return NULL;
    }

    return physicalDevice;
}