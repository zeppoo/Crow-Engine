#include "Vulkan/BackendInterface.hpp"
#include "Config/SettingsManager.hpp"
#include "Logger.hpp"
#include <memory>

namespace vulkan
{

  VulkanModule::VulkanModule(std::unique_ptr<core::Window> &window) : window{window}
  {
    Info info{0, "Peter", {0, 0, 0}};

    logger::Info("Setting up VulkanModule");
    if (Startup() == true) {
      logger::Info("Vulkan Module Succesfully Created!");
    }
  }

  VulkanModule::~VulkanModule()
  {

  }

  bool VulkanModule::Startup()
  {
    logger::Info("Setting Up QueueManager...");
    queueManager = std::make_unique<QueueManager>();

    logger::Info("Setting Up Device...");
    device = std::make_unique<Device>(window, queueManager);

    logger::Info("Setting Up SwapChain...");
    swapchain = std::make_unique<SwapChain>(device, queueManager);

    logger::Info("Setting Up PipelineManager");
    pipelineManager = std::make_unique<PipelineManager>(device, swapchain);

    CreateNewGraphicsPipeline();
    return true;
  }

  void VulkanModule::ShutDown()
  {
    logger::Warning("Waiting for Vulkan Device to Idle...");
    vkDeviceWaitIdle(device->getDevice());
    logger::Info("Destroying Vulkan Objects");

    // Destroy the render pass
    vkDestroyRenderPass(device->getDevice(), swapchain->GetRenderPass(), nullptr);

    for (VkImageView imageView: swapchain->GetSwapchainImageViews()) {
      vkDestroyImageView(device->getDevice(), imageView, nullptr);
    }

    vkDestroySwapchainKHR(device->getDevice(), swapchain->GetSwapchain(), nullptr);

    for (int i = 0; i < queueManager->GetQueueFamilies().size(); i++) {
      vkDestroyCommandPool(device->getDevice(), queueManager->GetQueueFamilies()[i].commandPool, nullptr);
    }

    vkDestroyDevice(device->getDevice(), nullptr);

    if (settings::getEnableValidationLayers()) {
      DestroyDebugUtilsMessengerEXT(device->getVkInstance(), device->getDebugMessenger(), nullptr);
    }

    vkDestroySurfaceKHR(device->getVkInstance(), device->getSurface(), nullptr);
    vkDestroyInstance(device->getVkInstance(), nullptr);

    logger::Info("All Vulkan Objects Destroyed!");
  }

  void VulkanModule::RenderFrame()
  {

  }

  void VulkanModule::CreateNewGraphicsPipeline()
  {

  }

  void VulkanModule::CreateNewGraphicsPipeline(const char* pipelineConfigFile)
  {
    logger::Info("Creating New GraphicsPipeline...");
    logger::Info("Successfully created new pipeline");
  }

  void VulkanModule::RecreateSwapchain()
  {
    logger::Warning("Recreating SwapChain");
    swapchain->RecreateSwapChain();
    logger::Info("Swapchain Recreated");
  }

  void VulkanModule::RecreateGraphicsPipeline()
  {

  }
}