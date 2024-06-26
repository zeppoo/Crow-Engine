#pragma once

#include "../crow_lib.hpp"

namespace crowe{

void VulkanStartup();
void CleanupVulkan();

VkInstance getVkInstance();
VkSurfaceKHR getSurface();
const bool getEnableValidationLayers();
const std::vector<const char*>& getValidationLayers();
const std::vector<const char*>& getDeviceExtensions();

void InitVulkan();
bool checkValidationLayerSupport();
bool checkExtensionSupport(const std::vector<const char*>& requiredExtensions);
}
