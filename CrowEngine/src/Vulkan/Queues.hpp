#pragma once

#include "../crow_lib.hpp"
#include <optional>

namespace crowe
{
struct QueueFamilyIndices {
  VkQueue familyQueue;
  std::optional<uint32_t> GraphicsQueue;
  std::optional<uint32_t> ComputeQueue;
  std::optional<uint32_t> TransferQueue;
  VkCommandPool commandPool;

  bool isComplete()
  { return
        GraphicsQueue.has_value() &&
        ComputeQueue.has_value() &&
        TransferQueue.has_value();
  }
};

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicDevice, VkSurfaceKHR surface);

}
