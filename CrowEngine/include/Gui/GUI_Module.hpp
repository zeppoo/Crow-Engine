#pragma once

#include "Gui/Gui_Defaults.hpp"
#include "Core/Window.hpp"
#include "Vulkan/BackendInterface.hpp"
#include "Vulkan/SettingsInterface.hpp"

namespace GUI
{
  class GUIModule {
  public:
    GUIModule(std::unique_ptr<core::Window> &window) : window{window} {}

    void InitializeImGUI();
    void ImplementVulkan(std::unique_ptr<vulkan::VulkanModule> vulkanModule);

  private:
    std::unique_ptr<core::Window> &window;
  };
}