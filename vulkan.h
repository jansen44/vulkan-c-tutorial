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

// Extensions related functions
struct VKExtensions listAvailableVKExtensions(void);
int checkForMissingNecessaryExtensions(struct VKExtensions);

// Validation layers related functions
struct VKValidationLayers listAvailableVKValidationLayers(void);
int checkValidationLayerSupport(struct VKValidationLayers);

// Initializers
VkInstance* initVulkanInstance(void);
VkPhysicalDevice* initPhysicalDevice(VkInstance*);
VkDevice* initLogicalDevice(VkPhysicalDevice*);

#endif