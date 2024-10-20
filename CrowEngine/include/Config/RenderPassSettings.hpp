#pragma once
#include "crow_lib.hpp"
#include "cppr.hpp"

struct RenderPassInfo
{
  std::vector<VkAttachmentDescription> attachments;
  std::vector<VkSubpassDescription> subpasses;
  std::vector<VkAttachmentReference> attachmentRefs;
  //std::vector<VkSubpassDependency> dependencies;
};

struct RenderPassAttachmentInfo
{
  REFLECT()

  VkFormat format = VK_FORMAT_UNDEFINED;                             // Undefined by default, should be set based on the target
  VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;             // Default to no multisampling (1 sample per pixel)
  VkAttachmentLoadOp loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;           // Clear the attachment at the start of the render pass
  VkAttachmentStoreOp storeOp = VK_ATTACHMENT_STORE_OP_STORE;        // Store the attachment contents after rendering
  VkAttachmentLoadOp stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE; // No need to load stencil data by default
  VkAttachmentStoreOp stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; // No need to store stencil data by default
  VkImageLayout initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;           // Undefined initial layout; Vulkan will handle transitions
  VkImageLayout finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;       // For color attachments, default to presenting to the screen

  uint32_t attachment = 0; // Index of the attachment in the render pass
  VkImageLayout layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // Layout of the attachment during the subpass
};

REFLECT_STRUCT_BEGIN(RenderPassAttachmentInfo)
  REFLECT_STRUCT_MEMBER(format)
  REFLECT_STRUCT_MEMBER(samples)
  REFLECT_STRUCT_MEMBER(loadOp)
  REFLECT_STRUCT_MEMBER(storeOp)
  REFLECT_STRUCT_MEMBER(stencilLoadOp)
  REFLECT_STRUCT_MEMBER(stencilStoreOp)
  REFLECT_STRUCT_MEMBER(initialLayout)
  REFLECT_STRUCT_MEMBER(finalLayout)
  REFLECT_STRUCT_MEMBER(attachment)
  REFLECT_STRUCT_MEMBER(layout)
REFLECT_STRUCT_END()

struct SubpassInfo
{
  REFLECT()

  VkPipelineBindPoint pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  uint32_t inputAttachmentCount = 0;
  const VkAttachmentReference* pInputAttachments = nullptr;
  uint32_t colorAttachmentCount = 0;
  const VkAttachmentReference* pColorAttachments = nullptr;
  const VkAttachmentReference* pDepthStencilAttachment = nullptr;
  const VkAttachmentReference* pResolveAttachments = nullptr;
  uint32_t preserveAttachmentCount = 0;
  const uint32_t* pPreserveAttachments = nullptr;

  /*// Subpass Dependencies
  uint32_t srcSubpass = 0; // Source subpass index (use VK_SUBPASS_EXTERNAL for external)
  uint32_t dstSubpass = 0; // Destination subpass index
  VkPipelineStageFlags srcStageMask = 0; // Source pipeline stage mask
  VkPipelineStageFlags dstStageMask = 0; // Destination pipeline stage mask
  VkAccessFlags srcAccessMask = 0; // Source access mask
  VkAccessFlags dstAccessMask = 0; // Destination access mask
  VkDependencyFlags dependencyFlags = 0; // Dependency flags (e.g., VK_DEPENDENCY_BY_REGION_BIT)*/
};

REFLECT_STRUCT_BEGIN(SubpassInfo)
  REFLECT_STRUCT_MEMBER(pipelineBindPoint)
  REFLECT_STRUCT_MEMBER(inputAttachmentCount)
  REFLECT_STRUCT_MEMBER(pInputAttachments)
  REFLECT_STRUCT_MEMBER(colorAttachmentCount)
  REFLECT_STRUCT_MEMBER(pColorAttachments)
  REFLECT_STRUCT_MEMBER(pDepthStencilAttachment)
  REFLECT_STRUCT_MEMBER(pResolveAttachments)
  REFLECT_STRUCT_MEMBER(preserveAttachmentCount)
  REFLECT_STRUCT_MEMBER(pPreserveAttachments)
  /*REFLECT_STRUCT_MEMBER(srcSubpass)
  REFLECT_STRUCT_MEMBER(dstSubpass)
  REFLECT_STRUCT_MEMBER(srcStageMask)
  REFLECT_STRUCT_MEMBER(dstStageMask)
  REFLECT_STRUCT_MEMBER(srcAccessMask)
  REFLECT_STRUCT_MEMBER(dstAccessMask)
  REFLECT_STRUCT_MEMBER(dependencyFlags)*/
REFLECT_STRUCT_END()

struct RenderPassConfig
{
  REFLECT()
  std::string name = "default";

  std::vector<RenderPassAttachmentInfo> attachments;
  std::vector<SubpassInfo> subpasses;

  RenderPassConfig(std::string name , const uint8_t attachmentCount, const uint8_t subpassCount) : name{name} {
    attachments.resize(attachmentCount);
    subpasses.resize(subpassCount);
  }
};

REFLECT_STRUCT_BEGIN(RenderPassConfig)
  REFLECT_STRUCT_MEMBER(name)
REFLECT_STRUCT_END()