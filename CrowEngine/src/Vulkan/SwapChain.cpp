#include "Vulkan/SwapChain.hpp"
#include <algorithm>
#include <limits>
#include "ConfigPaths.hpp"
#include "Logger.hpp"
#include "Utils/FileUtilities.hpp"
#include "Utils/GenerateDefaults.hpp"
#include "Vulkan/VulkanUtilities.hpp"
#include "cppr.hpp"

namespace vulkan
{
SwapChain::SwapChain(std::unique_ptr<Device> &device, std::unique_ptr<QueueManager> &queueManager) :
    device{device}, queueManager{queueManager}
{
  SetupSwapChain();
}

SwapChain::~SwapChain() {}

void SwapChain::SetupSwapChain()
{
  createSwapChain(VK_NULL_HANDLE);
  RenderPassConfig renderPassConfig = GenerateDefaultRenderPassConfig();
  DeserializeStructsFromFile(DefaultRenderpass_json, &renderPassConfig);
  CreateRenderPass(renderPassConfig);
  CreateSwapchainImages();
}

void SwapChain::CleanupSwapChain() {}

void SwapChain::RecreateSwapChain()
{
  vkDeviceWaitIdle(device->GetDevice());
  CleanupSwapChain();

  createSwapChain(swapchain);
}

void SwapChain::createSwapChain(VkSwapchainKHR oldSwapChain)
{
  SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device->GetPhysicDevice(), device->GetSurface());

  VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
  VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
  VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

  uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
  if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
    imageCount = swapChainSupport.capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = device->GetSurface();

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

  if (queueFamilyIndices.size() == 1) {
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  } else {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
  }

  createInfo.queueFamilyIndexCount = queueFamilyIndices.size();
  createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
  createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;
  createInfo.oldSwapchain = oldSwapChain;

  if (vkCreateSwapchainKHR(device->GetDevice(), &createInfo, nullptr, &swapchain) != VK_SUCCESS) {
    logger::FatalError("Failed to create SwapChain");
  }

  vkDestroySwapchainKHR(device->GetDevice(), oldSwapChain, nullptr);
  swapchainImageFormat = surfaceFormat.format;
  swapchainExtent = extent;

  logger::Info("SwapChain is setup!");
}

  void SwapChain::CreateSwapchainImages()
  {
    // Step 1: Get the number of swapchain images
    uint32_t imageCount;
    vkGetSwapchainImagesKHR(device->GetDevice(), swapchain, &imageCount, nullptr);
    swapchainImages.resize(imageCount);

    std::vector<VkImage> images(imageCount);
    vkGetSwapchainImagesKHR(device->GetDevice(), swapchain, &imageCount, images.data());
    // Store each image in your struct
    for (size_t i = 0; i < imageCount; i++) {
      swapchainImages[i].image = images[i];
      CreateImageView(i);
      CreateFrameBuffer(i);
    }
  }

  void SwapChain::CreateImageView(uint32_t index)
  {
    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = swapchainImages[index].image;
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

    if (vkCreateImageView(device->GetDevice(), &createInfo, nullptr, &swapchainImages[index].view) != VK_SUCCESS) {
      logger::FatalError("Failed to create Image Views");
    }
    logger::Info("Created Image View");
  }

  void SwapChain::CreateFrameBuffer(uint32_t index)
  {
      VkImageView attachments[] = {swapchainImages[index].view};

      VkFramebufferCreateInfo framebufferInfo{};
      framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      framebufferInfo.renderPass = renderPasses[0];
      framebufferInfo.attachmentCount = 1;
      framebufferInfo.pAttachments = attachments;
      framebufferInfo.width = swapchainExtent.width;
      framebufferInfo.height = swapchainExtent.height;
      framebufferInfo.layers = 1;

      if (vkCreateFramebuffer(device->GetDevice(), &framebufferInfo, nullptr, &swapchainImages[index].framebuffer) !=
          VK_SUCCESS) {
        throw std::runtime_error("failed to create framebuffer!");
      }
  }

void SwapChain::CreateRenderPass(RenderPassConfig &renderPassConfig)
{
  VkRenderPass renderPass;
  RenderPassInfo renderPassInfo{};

  logger::Info("Creating RenderPass...");

  for (RenderPassAttachmentInfo &attachmentInfo: renderPassConfig.attachments) {
    VkAttachmentDescription attachment{};
    attachment.format = swapchainImageFormat;
    attachment.samples = attachmentInfo.samples;
    attachment.loadOp = attachmentInfo.loadOp;
    attachment.storeOp = attachmentInfo.storeOp;
    attachment.stencilLoadOp = attachmentInfo.stencilLoadOp;
    attachment.stencilStoreOp = attachmentInfo.stencilStoreOp;
    attachment.initialLayout = attachmentInfo.initialLayout;
    attachment.finalLayout = attachmentInfo.finalLayout;

    VkAttachmentReference attachmentRef{};
    attachmentRef.attachment = attachmentInfo.attachment;
    attachmentRef.layout = attachmentInfo.layout;

    renderPassInfo.attachments.push_back(attachment);
    renderPassInfo.attachmentRefs.push_back(attachmentRef);
    attachmentInfo.reference = attachmentRef;
  }

  for (SubpassInfo &subpassInfo: renderPassConfig.subpasses) {
    for (RenderPassAttachmentInfo *attachmentInfo: subpassInfo.attachments)
      subpassInfo.BindAttachment(attachmentInfo);

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = subpassInfo.pipelineBindPoint;
    subpass.inputAttachmentCount = subpassInfo.inputAttachmentCount;
    subpass.pInputAttachments = subpassInfo.InputAttachments.data();
    subpass.colorAttachmentCount = subpassInfo.colorAttachmentCount;
    subpass.pColorAttachments = subpassInfo.ColorAttachments.data();
    subpass.pResolveAttachments = subpassInfo.ResolveAttachments.data();
    subpass.pDepthStencilAttachment = subpassInfo.DepthStencilAttachment.data();
    subpass.preserveAttachmentCount = subpassInfo.preserveAttachmentCount;
    subpass.pPreserveAttachments = subpassInfo.pPreserveAttachments;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = subpassInfo.srcSubpass;
    dependency.dstSubpass = subpassInfo.dstSubpass;
    dependency.srcStageMask = subpassInfo.srcStageMask;
    dependency.srcAccessMask = subpassInfo.srcAccessMask;
    dependency.dstStageMask = subpassInfo.dstStageMask;
    dependency.dstAccessMask = subpassInfo.dstAccessMask;

    renderPassInfo.subpasses.push_back(subpass);
    renderPassInfo.dependencies.push_back(dependency);
  }

  VkRenderPassCreateInfo renderPassCreateInfo{};
  renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassCreateInfo.attachmentCount = renderPassInfo.attachments.size();
  renderPassCreateInfo.pAttachments = renderPassInfo.attachments.data();
  renderPassCreateInfo.subpassCount = renderPassInfo.subpasses.size();
  renderPassCreateInfo.pSubpasses = renderPassInfo.subpasses.data();
  renderPassCreateInfo.dependencyCount = renderPassInfo.dependencies.size();
  renderPassCreateInfo.pDependencies = renderPassInfo.dependencies.data();

  if (vkCreateRenderPass(device->GetDevice(), &renderPassCreateInfo, nullptr, &renderPass) != VK_SUCCESS) {
    throw std::runtime_error("failed to create render pass!");
  }

  logger::Info("Successfully Created RenderPass!");
  renderPasses.push_back(renderPass);
}

void SwapChain::BeginRenderPass(VkCommandBuffer *pCommandBuffer, uint32_t renderpassIndex, uint32_t currentImage)
{
  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = renderPasses[renderpassIndex];
  renderPassInfo.framebuffer = swapchainImages[currentImage].framebuffer;
  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = swapchainExtent;
  VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
  renderPassInfo.clearValueCount = 1;
  renderPassInfo.pClearValues = &clearColor;
  vkCmdBeginRenderPass(*pCommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void SwapChain::SetViewPort(VkCommandBuffer *pCommandBuffer)
{
  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = (float) swapchainExtent.width;
  viewport.height = (float) swapchainExtent.height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(*pCommandBuffer, 0, 1, &viewport);
}
void SwapChain::SetScissor(VkCommandBuffer *pCommandBuffer)
{

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = swapchainExtent;
  vkCmdSetScissor(*pCommandBuffer, 0, 1, &scissor);
}

void SwapChain::EndRenderPass(VkCommandBuffer *pCommandBuffer) { vkCmdEndRenderPass(*pCommandBuffer); }


VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
{
  for (const auto &availableFormat: availableFormats) {
    if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return availableFormat;
    }
  }
  return availableFormats[0];
}

VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes)
{
  for (const auto &availablePresentMode: availablePresentModes) {
    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return availablePresentMode;
    }
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities)
{
  if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
    return capabilities.currentExtent;
  } else {
    int width, height;
    // glfwGetFramebufferSize(getWindow(), &width, &height);

    VkExtent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

    actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    return actualExtent;
  }
}
} // namespace vulkan
