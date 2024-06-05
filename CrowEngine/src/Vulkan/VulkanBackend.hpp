#ifndef VULKANBACKEND
#define VULKANBACKEND

#include "../crow_lib.hpp"

namespace crowe{

void VulkanStartup();

VkInstance getVkInstance();
VkDevice getLogicDevice();
VkPhysicalDevice getPhysicDevice();
VkSurfaceKHR getSurface();
VkSwapchainKHR getSwapchain();

void InitVulkan();
bool checkValidationLayerSupport();
std::vector<const char *> getRequiredExtensions();
void setupDebugMessenger();

}

#endif //VULKANBACKEND
