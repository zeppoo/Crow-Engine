#pragma once

#include "../crow_lib.hpp"
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

    struct PipelineInfo {

    };

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicDevice, VkSurfaceKHR surface);
}
