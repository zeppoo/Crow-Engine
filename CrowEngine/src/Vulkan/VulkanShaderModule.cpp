#include "VulkanShaderModule.hpp"
#include "VulkanUtilities.hpp"

namespace crowe
{
  VulkanShaderModule::VulkanShaderModule(VulkanDevice &device, std::string filepath) : device{device}, filePath{filepath}
  {
    createShaderModule();
  }

  VulkanShaderModule::~VulkanShaderModule()
  {
    vkDestroyShaderModule(device.getDevice(), shaderModule, nullptr);
  }

  void VulkanShaderModule::createShaderModule()
  {
    auto shaderCode = readFile(filePath);

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = shaderCode.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(shaderCode.data());

    if (vkCreateShaderModule(device.getDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
    {
      throw std::runtime_error("failed to create shader module!");
    }
  }

  void VulkanShaderModule::createShaderStageInfo(VkShaderStageFlagBits shaderBit)
  {
    shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageCreateInfo.stage = shaderBit;
    shaderStageCreateInfo.module = shaderModule;
    shaderStageCreateInfo.pName = "main";
  }
}