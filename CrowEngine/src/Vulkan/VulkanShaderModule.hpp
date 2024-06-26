#pragma once

#include "../crow_lib.hpp"
#include "VulkanDevice.hpp"

namespace crowe
{
  class VulkanShaderModule
  {
  public:
    VulkanShaderModule(VulkanDevice &device, std::string filepath);
    ~VulkanShaderModule();

    void createShaderStageInfo(VkShaderStageFlagBits shaderBit);

    const std::string filePath;
    VkShaderModule shaderModule;
    VkPipelineShaderStageCreateInfo shaderStageCreateInfo;

  private:
    void createShaderModule();

    VulkanDevice &device;
  };
}