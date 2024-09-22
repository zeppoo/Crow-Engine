#include "VulkanShaderModule.hpp"
#include "../crow_utils.hpp"

namespace crowe
{
  VulkanShaderModule::VulkanShaderModule(VulkanDevice &device, s) : device{device}, filePath{filepath}
  {
    createShaderModule();
  }

  VulkanShaderModule::~VulkanShaderModule()
  {

  }




}