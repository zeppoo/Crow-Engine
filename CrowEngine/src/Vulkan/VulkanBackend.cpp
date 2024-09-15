#include "VulkanBackend.hpp"
#include "../Config/SettingsManager.hpp"
#include "../Logger.hpp"
#include <memory>

namespace crowe
{

VulkanModule::VulkanModule(std::unique_ptr<Window>& window) : window{window}
{
  INFO("Setting up VulkanModule");
  if (VulkanStartup() == true)
  {
    INFO("Vulkan Module Succesfully Created!");
  }
}

VulkanModule::~VulkanModule()
{

}

bool VulkanModule::VulkanStartup()
{
  INFO("Setting Up VulkanQueueManager...");
  queueManager = std::make_unique<VulkanQueueManager>();
  INFO("Setting Up VulkanDevice...");
  device = std::make_unique<VulkanDevice>(window, queueManager);

  return true;
}

void VulkanModule::VulkanShutDown()
{
  WARNING("Waiting for Vulkan Device to Idle...");
  vkDeviceWaitIdle(device->getDevice());
  INFO("Destroying Vulkan Objects");
  for(int i = 0; i < queueManager->GetQueueFamilies().size(); i++)
  {
    vkDestroyCommandPool(device->getDevice(), queueManager->GetQueueFamilies()[i].commandPool, nullptr);
  }

  vkDestroyDevice(device->getDevice(), nullptr);

  if (getEnableValidationLayers()) {
    DestroyDebugUtilsMessengerEXT(device->getVkInstance(), device->getDebugMessenger(), nullptr);
  }

  vkDestroySurfaceKHR(device->getVkInstance(), device->getSurface(), nullptr);
  vkDestroyInstance(device->getVkInstance(), nullptr);

  INFO("All Vulkan Objects Destroyed!");
}

void VulkanModule::RecreateSwapchain() {}

void VulkanModule::RecreateGraphicsPipeline() {}
}