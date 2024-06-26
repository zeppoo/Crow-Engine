#pragma once

#include "../crow_lib.hpp"
#include <fstream>
#include <optional>

namespace crowe
{
    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> computeFamily;
        std::optional<uint32_t> transferFamily;

        bool isComplete()
        { return
              graphicsFamily.has_value() &&
              computeFamily.has_value() &&
              transferFamily.has_value();
        }
    };

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicDevice, VkSurfaceKHR surface);
    std::vector<char> readFile(const std::string &filepath);
}
