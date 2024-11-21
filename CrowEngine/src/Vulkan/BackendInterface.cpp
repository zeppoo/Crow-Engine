#include "Vulkan/BackendInterface.hpp"
#include "Config/SettingsManager.hpp"
#include "Logger.hpp"
#include <memory>

namespace vulkan
{

  VulkanModule::VulkanModule(std::unique_ptr<core::Window> &window) : window{window}
  {
    Info info{0, "Peter", {0, 0, 0}};

    log::Info("Setting up VulkanModule");
    if (Startup() == true) {
      log::Info("Vulkan Module Succesfully Created!");
    }
  }

  VulkanModule::~VulkanModule()
  {

  }

  bool VulkanModule::Startup()
  {
    log::Info("Setting Up QueueManager...");
    queueManager = std::make_unique<QueueManager>();

    log::Info("Setting Up Device...");
    device = std::make_unique<Device>(window, queueManager);

    log::Info("Setting Up SwapChain...");
    swapchain = std::make_unique<SwapChain>(device, queueManager);

    CreateNewGraphicsPipeline();
    return true;
  }

  void VulkanModule::ShutDown()
  {
    log::Warning("Waiting for Vulkan Device to Idle...");
    vkDeviceWaitIdle(device->getDevice());
    log::Info("Destroying Vulkan Objects");

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

    log::Info("All Vulkan Objects Destroyed!");
  }

  void VulkanModule::RenderFrame()
  {

  }

  void VulkanModule::CreateNewGraphicsPipeline()
  {
    PipelineSettings pipelineSettings{};
    PipelineInfo pipelineInfo;
    pipelineInfo = CreatePipelineInfo(pipelineSettings, swapchain->GetSwapchainExtent());
    log::Info("Creating New GraphicsPipeline...");
    std::unique_ptr<GraphicsPipeline> graphicsPipeline = std::make_unique<GraphicsPipeline>(device, swapchain);
    graphicsPipeline->CreateGraphicsPipeline(pipelineInfo);
    graphicsPipelines.push_back(std::move(graphicsPipeline));
    log::Info("Successfully created new pipeline");
  }

  void VulkanModule::CreateNewGraphicsPipeline(const char* pipelineConfigFile)
  {
    PipelineSettings pipelineSettings{};
    pipelineSettings.structDesc.From_Json(pipelineConfigFile, &pipelineSettings);
    PipelineInfo pipelineInfo;
    pipelineInfo = CreatePipelineInfo(pipelineSettings, swapchain->GetSwapchainExtent());
    log::Info("Creating New GraphicsPipeline...");
    std::unique_ptr<GraphicsPipeline> graphicsPipeline = std::make_unique<GraphicsPipeline>(device, swapchain);
    graphicsPipeline->CreateGraphicsPipeline(pipelineInfo);
    graphicsPipelines.push_back(std::move(graphicsPipeline));
    log::Info("Successfully created new pipeline");
  }

  void VulkanModule::RecreateSwapchain()
  {
    log::Warning("Recreating SwapChain");
    swapchain->RecreateSwapChain();
    log::Info("Swapchain Recreated");
  }

  void VulkanModule::RecreateGraphicsPipeline(std::unique_ptr<GraphicsPipeline> graphicsPipeline)
  {

  }
}