#pragma once
#include "crow_lib.hpp"
#include "cppr.hpp"

namespace configs
{
  struct VulkanConfig
  {
    REFLECT()

    std::string nameID;
    bool enableValidationLayers = true;

    // Device
    bool PreferDiscreteGPU = true;
    uint8_t PresentQueuePreference = 1;
    uint8_t GraphicsQueuePreference = 1;
    uint8_t ComputeQueuePreference = 1;
    uint8_t TransferQueuePreference = 1;

    // Swapchain
    uint8_t max_frames_in_flight = 3;
    uint16_t viewportHeight;
    uint16_t viewportWidth;
    VkPresentModeKHR preferredPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
  };

  REFLECT_STRUCT_BEGIN(VulkanConfig)
    REFLECT_STRUCT_MEMBER(nameID)
    REFLECT_STRUCT_MEMBER(enableValidationLayers)
    REFLECT_STRUCT_MEMBER(PreferDiscreteGPU)
    REFLECT_STRUCT_MEMBER(PresentQueuePreference)
    REFLECT_STRUCT_MEMBER(GraphicsQueuePreference)
    REFLECT_STRUCT_MEMBER(ComputeQueuePreference)
    REFLECT_STRUCT_MEMBER(TransferQueuePreference)
    REFLECT_STRUCT_MEMBER(max_frames_in_flight)
    REFLECT_STRUCT_MEMBER(viewportHeight)
    REFLECT_STRUCT_MEMBER(viewportWidth)
    REFLECT_STRUCT_MEMBER(preferredPresentMode)
  REFLECT_STRUCT_END()
}