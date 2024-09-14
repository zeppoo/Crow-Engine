#include "VulkanSwapChain.hpp"
#include "VulkanUtilities.hpp"
#include "VulkanDevice.hpp"
#include "VulkanBackend.hpp"
#include "../Core/Window.hpp"

#include <limits>
#include <algorithm>

namespace crowe
{
  VulkanSwapChain::VulkanSwapChain(VulkanDevice &device, VulkanQueueManager& queueManager) : device{device}, queueManager{queueManager}
  {
    std::cout << "Setting up Vulkan Device" << std::endl;
    createSwapChain();
    //createRenderPass();
  }

  VulkanSwapChain::~VulkanSwapChain()
  {

  }

  void VulkanSwapChain::recreateSwapChain()
  {

  }

  void VulkanSwapChain::createSwapChain()
  {
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device.getPhysicDevice(), device.getSurface());

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
    {
      imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = device.getSurface();

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;
    createInfo.pQueueFamilyIndices = nullptr;
    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(device.getDevice(), &createInfo, nullptr, &swapchain) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(device.getDevice(), swapchain, &imageCount, nullptr);
    swapchainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device.getDevice(), swapchain, &imageCount, swapchainImages.data());

    swapchainImageFormat = surfaceFormat.format;
    swapchainExtent = extent;

    std::cout << "Vulkan Device is Setup" << std::endl;
  }

  void VulkanSwapChain::createRenderPass()
  {
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = swapchainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(device.getDevice(), &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to create render pass!");
    }
  }

  void VulkanSwapChain::createImageViews()
  {
    swapchainImageViews.resize(swapchainImages.size());
    for (size_t i = 0; i < swapchainImages.size(); i++)
    {
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

      if (vkCreateImageView(device.getDevice(), &createInfo, nullptr, &swapchainImageViews[i]) != VK_SUCCESS)
      {
        throw std::runtime_error("failed to create image views!");
      }
    }
  }

  void VulkanSwapChain::createFramebuffers()
  {
    swapchainFramebuffers.resize(swapchainImageViews.size());

    for (size_t i = 0; i < swapchainImageViews.size(); i++)
    {
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

      if (vkCreateFramebuffer(device.getDevice(), &framebufferInfo, nullptr, &swapchainFramebuffers[i]) !=
          VK_SUCCESS)
      {
        throw std::runtime_error("failed to create framebuffer!");
      }
    }
  }

  VkSurfaceFormatKHR VulkanSwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
  {
    for (const auto &availableFormat: availableFormats)
    {
      if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
          availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
      {
        return availableFormat;
      }
    }
    return availableFormats[0];
  }

  VkPresentModeKHR VulkanSwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes)
  {
    for (const auto &availablePresentMode: availablePresentModes)
    {
      if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
      {
        return availablePresentMode;
      }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
  }

  VkExtent2D VulkanSwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities)
  {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
      return capabilities.currentExtent;
    } else
    {
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