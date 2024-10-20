#include "Vulkan/VulkanSwapChain.hpp"
#include "Logger.hpp"
#include "Vulkan/VulkanUtilities.hpp"
#include "Utils/FileUtilities.hpp"
#include "cppr.hpp"
#include "ConfigPaths.hpp"
#include "Utils/GenerateDefaults.hpp"
#include <limits>
#include <algorithm>

namespace crowe
{
  VulkanSwapChain::VulkanSwapChain(std::unique_ptr<VulkanDevice> &device, std::unique_ptr<VulkanQueueManager> &queueManager) : device{device}, queueManager{queueManager}
  {
    SetupSwapChain();
  }

  VulkanSwapChain::~VulkanSwapChain()
  {

  }

  void VulkanSwapChain::SetupSwapChain()
  {
    createSwapChain(VK_NULL_HANDLE);
    createImageViews();
    RenderPassConfig renderPassConfig = GenerateDefaultRenderPassConfig();
    DeserializeStructsFromFile(DefaultRenderpass_json, &renderPassConfig);
    CreateRenderPass(renderPassConfig);
  }

  void VulkanSwapChain::CleanupSwapChain()
  {
    //for (size_t i = 0; i < swapchainFramebuffers.size(); i++) {
    //  vkDestroyFramebuffer(device->getDevice(), swapchainFramebuffers[i], nullptr);
    //}

    for (size_t i = 0; i < swapchainImageViews.size(); i++) {
      vkDestroyImageView(device->getDevice(), swapchainImageViews[i], nullptr);
    }
  }

  void VulkanSwapChain::RecreateSwapChain()
  {
    vkDeviceWaitIdle(device->getDevice());
    CleanupSwapChain();

    createSwapChain(swapchain);
    createImageViews();
    //createFramebuffers();
  }

  void VulkanSwapChain::createSwapChain(VkSwapchainKHR oldSwapChain)
  {
    SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device->getPhysicDevice(), device->getSurface());

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
      imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = device->getSurface();

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    std::vector<uint32_t> queueFamilyIndices;

    for (QueueData queueData: queueManager->GetPresentQueues()) {
      queueFamilyIndices.push_back(queueData.familyIndex);
    }
    for (QueueData queueData: queueManager->GetGraphicsQueues()) {
      queueFamilyIndices.push_back(queueData.familyIndex);
    }

    RemoveDuplicatesInList(queueFamilyIndices);

    if (queueFamilyIndices.size() == 1) { createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; }
    else { createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT; }

    createInfo.queueFamilyIndexCount = queueFamilyIndices.size();
    createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = oldSwapChain;

    if (vkCreateSwapchainKHR(device->getDevice(), &createInfo, nullptr, &swapchain) != VK_SUCCESS) {
      FATAL_ERROR("Failed to create SwapChain");
    }
    vkDestroySwapchainKHR(device->getDevice(), oldSwapChain, nullptr);

    vkGetSwapchainImagesKHR(device->getDevice(), swapchain, &imageCount, nullptr);
    swapchainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device->getDevice(), swapchain, &imageCount, swapchainImages.data());

    swapchainImageFormat = surfaceFormat.format;
    swapchainExtent = extent;

    INFO("SwapChain is setup!");
  }

  void VulkanSwapChain::createImageViews()
  {
    swapchainImageViews.resize(swapchainImages.size());
    for (size_t i = 0; i < swapchainImages.size(); i++) {
      VkImageViewCreateInfo createInfo{};
      createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      createInfo.image = swapchainImages[i];
      createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
      createInfo.format = swapchainImageFormat;
      createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
      createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      createInfo.subresourceRange.baseMipLevel = 0;
      createInfo.subresourceRange.levelCount = 1;
      createInfo.subresourceRange.baseArrayLayer = 0;
      createInfo.subresourceRange.layerCount = 1;

      if (vkCreateImageView(device->getDevice(), &createInfo, nullptr, &swapchainImageViews[i]) != VK_SUCCESS) {
        FATAL_ERROR("Failed to create Image Views");
      }

      INFO("Created Image View");
    }
  }

  void VulkanSwapChain::CreateRenderPass(RenderPassConfig& renderPassConfig)
  {
    RenderPassInfo renderPassInfo{};

    INFO("Creating RenderPass...");

    for(RenderPassAttachmentInfo info : renderPassConfig.attachments)
    {
      VkAttachmentDescription attachment{};
      attachment.format = swapchainImageFormat;
      attachment.samples = info.samples;
      attachment.loadOp = info.loadOp;
      attachment.storeOp = info.storeOp;
      attachment.stencilLoadOp = info.stencilLoadOp;
      attachment.stencilStoreOp = info.stencilStoreOp;
      attachment.initialLayout = info.initialLayout;
      attachment.finalLayout = info.finalLayout;

      VkAttachmentReference attachmentRef{};
      attachmentRef.attachment = info.attachment;
      attachmentRef.layout = info.layout;

      renderPassInfo.attachments.push_back(attachment);
      renderPassInfo.attachmentRefs.push_back(attachmentRef);
    }

    for(SubpassInfo info : renderPassConfig.subpasses)
    {
      VkSubpassDescription subpass{};
      subpass.pipelineBindPoint = info.pipelineBindPoint;
      subpass.inputAttachmentCount = info.inputAttachmentCount;
      subpass.pInputAttachments = info.pInputAttachments;
      subpass.colorAttachmentCount = info.colorAttachmentCount;
      subpass.pColorAttachments = info.pColorAttachments;
      subpass.pResolveAttachments = info.pResolveAttachments;
      subpass.pDepthStencilAttachment = info.pDepthStencilAttachment;
      subpass.preserveAttachmentCount = info.preserveAttachmentCount;
      subpass.pPreserveAttachments = info.pPreserveAttachments;

      /*VkSubpassDependency dependency{};
      dependency.srcSubpass = info.srcSubpass;
      dependency.dstSubpass = info.dstSubpass;
      dependency.srcStageMask = info.srcStageMask;
      dependency.srcAccessMask = info.srcAccessMask;
      dependency.dstStageMask = info.dstStageMask;
      dependency.dstAccessMask = info.dstAccessMask;
      dependency.dependencyFlags = info.dependencyFlags;

      renderPassInfo.subpasses.push_back(subpass);
      renderPassInfo.dependencies.push_back(dependency);*/
    }

    VkRenderPassCreateInfo renderPassCreateInfo{};
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.attachmentCount = renderPassInfo.attachments.size();
    renderPassCreateInfo.pAttachments = renderPassInfo.attachments.data();
    renderPassCreateInfo.subpassCount = renderPassInfo.subpasses.size();
    renderPassCreateInfo.pSubpasses = renderPassInfo.subpasses.data();
    //renderPassCreateInfo.dependencyCount = renderPassInfo.dependencies.size();
    //renderPassCreateInfo.pDependencies = renderPassInfo.dependencies.data();

    if (vkCreateRenderPass(device->getDevice(), &renderPassCreateInfo, nullptr, &renderPass) != VK_SUCCESS) {
      throw std::runtime_error("failed to create render pass!");
    }

    INFO("Successfully Created RenderPass!");
  }

  void VulkanSwapChain::createFramebuffers()
  {
    swapchainFramebuffers.resize(swapchainImageViews.size());

    for (size_t i = 0; i < swapchainImageViews.size(); i++) {
      VkImageView attachments[] = {
          swapchainImageViews[i]
      };

      VkFramebufferCreateInfo framebufferInfo{};
      framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      framebufferInfo.renderPass = renderPass;
      framebufferInfo.attachmentCount = 1;
      framebufferInfo.pAttachments = attachments;
      framebufferInfo.width = swapchainExtent.width;
      framebufferInfo.height = swapchainExtent.height;
      framebufferInfo.layers = 1;

      if (vkCreateFramebuffer(device->getDevice(), &framebufferInfo, nullptr, &swapchainFramebuffers[i]) !=
          VK_SUCCESS) {
        throw std::runtime_error("failed to create framebuffer!");
      }
    }
  }

  VkSurfaceFormatKHR VulkanSwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
  {
    for (const auto &availableFormat: availableFormats) {
      if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
          availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
        return availableFormat;
      }
    }
    return availableFormats[0];
  }

  VkPresentModeKHR VulkanSwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes)
  {
    for (const auto &availablePresentMode: availablePresentModes) {
      if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
        return availablePresentMode;
      }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
  }

  VkExtent2D VulkanSwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities)
  {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
      return capabilities.currentExtent;
    } else {
      int width, height;
      //glfwGetFramebufferSize(getWindow(), &width, &height);

      VkExtent2D actualExtent = {
          static_cast<uint32_t>(width),
          static_cast<uint32_t>(height)
      };

      actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                                      capabilities.maxImageExtent.width);
      actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                                       capabilities.maxImageExtent.height);

      return actualExtent;
    }
  }
}