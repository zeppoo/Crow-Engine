#pragma once
#include "crow_lib.hpp"
#include "Logger.hpp"
#include "cppr.hpp"
#include <unordered_map>

enum AttachmentType {
  None = 0,
  Input,
  Color,
  Depth,
  Resolve
};

struct RenderPassInfo
{
  std::vector<VkAttachmentDescription> attachments;
  std::vector<VkAttachmentReference> attachmentRefs;
  std::vector<VkSubpassDescription> subpasses;
  //std::vector<VkSubpassDependency> dependencies;
};


struct RenderPassAttachmentInfo
{
  REFLECT()

  AttachmentType type = None;
  VkFormat format = VK_FORMAT_UNDEFINED;                             // Undefined by default, should be set based on the target
  VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;             // Default to no multisampling (1 sample per pixel)
  VkAttachmentLoadOp loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;           // Clear the attachment at the start of the render pass
  VkAttachmentStoreOp storeOp = VK_ATTACHMENT_STORE_OP_STORE;        // Store the attachment contents after rendering
  VkAttachmentLoadOp stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE; // No need to load stencil data by default
  VkAttachmentStoreOp stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; // No need to store stencil data by default
  VkImageLayout initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;           // Undefined initial layout; Vulkan will handle transitions
  VkImageLayout finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;       // For color attachments, default to presenting to the screen

  uint32_t attachment = 0;
  VkImageLayout layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // Layout of the attachment during the subpass

  VkAttachmentReference reference;
};

REFLECT_STRUCT_BEGIN(RenderPassAttachmentInfo)
  REFLECT_STRUCT_MEMBER(type)
  REFLECT_STRUCT_MEMBER(format)
  REFLECT_STRUCT_MEMBER(samples)
  REFLECT_STRUCT_MEMBER(loadOp)
  REFLECT_STRUCT_MEMBER(storeOp)
  REFLECT_STRUCT_MEMBER(stencilLoadOp)
  REFLECT_STRUCT_MEMBER(stencilStoreOp)
  REFLECT_STRUCT_MEMBER(initialLayout)
  REFLECT_STRUCT_MEMBER(finalLayout)
  REFLECT_STRUCT_MEMBER(layout)
REFLECT_STRUCT_END()

struct SubpassInfo
{
  REFLECT()

  VkPipelineBindPoint pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  uint32_t inputAttachmentCount = 0;
  std::vector<VkAttachmentReference> InputAttachments;
  uint32_t colorAttachmentCount = 0;
  std::vector<VkAttachmentReference> ColorAttachments;
  std::vector<VkAttachmentReference> DepthStencilAttachment;
  std::vector<VkAttachmentReference> ResolveAttachments;
  uint32_t preserveAttachmentCount = 0;
  uint32_t* pPreserveAttachments = nullptr;

  void BindAttachment(RenderPassAttachmentInfo &attachmentInfo)
  {
    AttachmentType type = attachmentInfo.type;
    switch (type) {
      case None:
        logger::Error("Attachment Type is None");
        break;
      case Input:
        inputAttachmentCount++;
        InputAttachments.push_back(attachmentInfo.reference);
        break;
      case Color:
        colorAttachmentCount++;
        ColorAttachments.push_back(attachmentInfo.reference);
        break;
      case Depth:
        DepthStencilAttachment.push_back(attachmentInfo.reference);
        break;
      case Resolve:
        ResolveAttachments.push_back(attachmentInfo.reference);
        break;
      default:
        logger::Error("No Attachment Type");
        break;
    }
  }

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

  std::vector<RenderPassAttachmentInfo> attachments = {};
  std::vector<SubpassInfo> subpasses = {};

  RenderPassConfig(std::string name) : name{name} {}

  void AddSubpass()
  {
    SubpassInfo subpass_info{};
    subpasses.push_back(subpass_info);
  }

  void AddSubpass(SubpassInfo subpassInfo)
  {
    subpasses.push_back(subpassInfo);
  }

  void AddAttachment(AttachmentType type)
  {
    RenderPassAttachmentInfo attachment_info{};
    attachment_info.type = type;
    attachment_info.attachment = attachments.size();
    attachments.push_back(attachment_info);
  }

  void AddAttachment(AttachmentType type, RenderPassAttachmentInfo attachment_info)
  {
    attachment_info.type = type;
    attachment_info.attachment = attachments.size();
    attachments.push_back(attachment_info);
  }
};

REFLECT_STRUCT_BEGIN(RenderPassConfig)
  REFLECT_STRUCT_MEMBER(name)
REFLECT_STRUCT_END()