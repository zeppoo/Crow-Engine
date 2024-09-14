#include "VulkanBackend.hpp"
#include <memory>

namespace crowe
{

VulkanModule::VulkanModule(std::unique_ptr<Window>& window) : window{window}
{
  VulkanStartup();
}

VulkanModule::~VulkanModule()
{

}

void VulkanModule::VulkanStartup()
{
  vulkanQueueManager = std::make_unique<VulkanQueueManager>();
  device = std::make_unique<VulkanDevice>(window, vulkanQueueManager);
}

void VulkanModule::CheckPhysicalDevice()
{
  device->FindPhysicalDevice();
}

void VulkanModule::RecreateLogicalDevice() {
  device->CreateLogicalDevice();
}

void VulkanModule::RecreateSwapchain() {}

void VulkanModule::RecreateGraphicsPipeline() {}
}