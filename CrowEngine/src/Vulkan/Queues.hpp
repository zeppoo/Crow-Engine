#ifndef QUEUES_HPP
#define QUEUES_HPP

#include "../crow_lib.hpp"
#include <optional>

namespace crowe
{
struct QueueFamilyIndices {
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;
  std::optional<uint32_t> transferFamily;

  bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
};

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicDevice, VkSurfaceKHR surface);

}
#endif //QUEUES_HPP
