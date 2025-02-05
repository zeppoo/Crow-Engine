#include "Vulkan/Vulkan_Module.hpp"
#include "Vulkan/Debugger.hpp"
#include "Config/SettingsManager.hpp"
#include "Utils/Logger.hpp"

namespace vulkan
{
  void VulkanModule::InitiliazeVulkan()
  {
    logger::Info("Setting Up Queue Manager...");
    queueManager = std::make_shared<QueueManager>();

    logger::Info("Setting Up Device...");
    device = std::make_shared<Device>(queueManager);

    logger::Info("Setting Up CommandBufferManager...");
    cmdBufferManager = std::make_shared<CommandBufferManager>(device, queueManager);

    logger::Info("Setting Up BufferManager...");
    bufferManager = std::make_shared<BufferManager>(device, cmdBufferManager);

    logger::Info("Setting Up Frame Manager...");
    frameManager = std::make_shared<FrameManager>(device, queueManager, swapchain);

    logger::Info("Setting Up SwapChain...");
    swapchain = std::make_shared<SwapChain>(device, queueManager);

    logger::Info("Setting Up PipelineManager...");
    pipelineManager = std::make_shared<PipelineManager>(device, swapchain, frameManager);

    CreateNewGraphicsPipeline();
  }

  void VulkanModule::ShutDown()
  {
    logger::Warning("Waiting for Vulkan Device to Idle...");
    vkDeviceWaitIdle(device->GetDevice());
    logger::Info("Destroying Vulkan Objects");

    for (GraphicsPipeline & pipeline : pipelineManager->GetGraphicsPipelines())
    {
      vkDestroyPipeline(device->GetDevice(), pipeline.pipeline, nullptr);
      vkDestroyPipelineLayout(device->GetDevice(), pipeline.pipelineLayout, nullptr);
    }

    for(SwapchainImage & image : swapchain->GetSwapchainImages()) {
      vkDestroyFramebuffer(device->GetDevice(), image.framebuffer, nullptr);
      vkDestroyImageView(device->GetDevice(), image.view, nullptr);
    }

    // Destroy the render pass
    for (VkRenderPass & renderPass : swapchain->GetRenderPasses()) {
      vkDestroyRenderPass(device->GetDevice(), renderPass, nullptr);
    }

    vkDestroySwapchainKHR(device->GetDevice(), swapchain->GetSwapchain(), nullptr);

    for (int i = 0; i < queueManager->GetQueueFamilies().size(); i++) {
      vkDestroyCommandPool(device->GetDevice(), queueManager->GetQueueFamilies()[i].commandPool, nullptr);
    }

    vkDestroyDevice(device->GetDevice(), nullptr);

    if (settings::getEnableValidationLayers()) {
      DestroyDebugUtilsMessengerEXT(device->GetVkInstance(), device->GetDebugMessenger(), nullptr);
    }

    vkDestroySurfaceKHR(device->GetVkInstance(), device->GetSurface(), nullptr);
    vkDestroyInstance(device->GetVkInstance(), nullptr);

    logger::Info("All Vulkan Objects Destroyed!");
  }
}