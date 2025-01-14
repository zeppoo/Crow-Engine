#include "Vulkan/BackendInterface.hpp"
#include "Config/SettingsManager.hpp"
#include "Logger.hpp"
#include <memory>

namespace vulkan
{
  VulkanModule::VulkanModule(std::shared_ptr<core::Window> &window) : window{window}
  {
    logger::Info("Setting up VulkanModule");
    if (InitiliazeVulkan() == true) {
      logger::Info("Vulkan Module Succesfully Created!");
    }
  }

  VulkanModule::~VulkanModule()
  {

  }

  bool VulkanModule::InitiliazeVulkan()
  {
    logger::Info("Setting Up Queue Manager...");
    queueManager = std::make_shared<QueueManager>();

    logger::Info("Setting Up Device...");
    device = std::make_shared<Device>(window, queueManager);

    logger::Info("Setting Up CommandBufferManager...");
    cmdBufferManager = std::make_shared<CommandBufferManager>(device, queueManager);

    logger::Info("Setting Up BufferManager...");
    bufferManager = std::make_shared<BufferManager>(device, queueManager);

    logger::Info("Setting Up Frame Manager...");
    frameManager = std::make_shared<FrameManager>(device, queueManager, swapchain);

    logger::Info("Setting Up SwapChain...");
    swapchain = std::make_shared<SwapChain>(device, queueManager);

    logger::Info("Setting Up PipelineManager...");
    pipelineManager = std::make_shared<PipelineManager>(device, swapchain, frameManager);

    CreateNewGraphicsPipeline();

    return true;
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

  void VulkanModule::RenderingLoop()
  {

  }

  void VulkanModule::BeginShaderExecution()
  {
    frameManager->PresentFrame(swapchain->GetSwapchain());
  }

  void VulkanModule::EndShaderExecution()
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

  void RemoveGraphicsPipeline()
  {

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