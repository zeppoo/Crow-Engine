#pragma once

#include "../crow_lib.hpp"

namespace crowe
{
  VkPhysicalDevice FindPhysicalDevice(VkInstance instance);
  void CreateLogicalDevice();


  bool checkDeviceExtensionSupport(VkPhysicalDevice physicDevice);
  bool CheckDeviceSuitability(VkPhysicalDevice device);
  int RateDevice(VkPhysicalDevice device);

} // namespace crowe
