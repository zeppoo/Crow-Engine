#pragma once

#include "../crow_lib.hpp"
#include <optional>

namespace crowe
{
struct QueueFamilyIndices {
  VkQueue familyQueue;
  std::vector<VkQueueFlagBits> Bits;
  std::optional<uint32_t> queue1;
  std::optional<uint32_t> queue2;
  VkCommandPool commandPool;

  bool isComplete()
  { return
        queue1.has_value() &&
        queue2.has_value();
  }
};

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicDevice, VkSurfaceKHR surface);

}
