#ifndef __VULKAN__HELPERS__
#define __VULKAN__HELPERS__

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef NDEBUG
#define ENABLE_VALIDATION_LAYERS
#define VK_VERBOSE
#endif

#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define WIN_TITLE "Vulkan Test: Hello triangle"

struct VKExtensions {
    VkExtensionProperties* extensions;
    uint32_t count;
};

struct VKValidationLayers {
    VkLayerProperties* layers;
    uint32_t count;
};

struct VKDeviceQueue {
    VkDevice device;
    VkQueue graphicsQueue;
};

struct VKSetup {
    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    struct VKDeviceQueue deviceQueue;
};

// Extensions related functions
struct VKExtensions listAvailableVKExtensions(void);
int checkForMissingNecessaryExtensions(struct VKExtensions);

// Validation layers related functions
struct VKValidationLayers listAvailableVKValidationLayers(void);
int checkValidationLayerSupport(struct VKValidationLayers);

// Initializers
struct VKSetup* initVulkan(void);
VkInstance* initVulkanInstance(void);
VkPhysicalDevice* initPhysicalDevice(VkInstance*);
struct VKDeviceQueue* initLogicalDevice(VkPhysicalDevice*);

// Garbage
void cleanVulkan(struct VKSetup);

#endif