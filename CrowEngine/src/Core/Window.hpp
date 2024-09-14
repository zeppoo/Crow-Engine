#pragma once

#include "../crow_lib.hpp"
#include "../config/SettingsManager.hpp"

namespace crowe
{
  class Window
  {
  public:
    Window();
    ~Window();
    GLFWwindow* GetWindow() { return window; }
    void InitializeGLFW();
    void InitializeWindow();
    VkSurfaceKHR CreateVulkanSurface(VkInstance instance);
    void windowLoop();
  private:

    SettingsManager& settings = SettingsManager::getInstance();
    GLFWwindow* window;
  };
}
