#include "VulkanBackend.hpp"
#include <memory>

namespace crowe
{

std::unique_ptr<VulkanQueueManager> vulkanQueueManager;
std::unique_ptr<VulkanDevice> device;
std::unique_ptr<VulkanSwapChain> swapchain;

//std::unique_ptr<VulkanGraphicsPipeline> pipeline;

VulkanModule::VulkanModule()
{
  VulkanStartup();
}

VulkanModule::~VulkanModule() {}

void VulkanModule::VulkanStartup()
{
  vulkanQueueManager = std::make_unique<VulkanQueueManager>();
  device = std::make_unique<VulkanDevice>(vulkanQueueManager);
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