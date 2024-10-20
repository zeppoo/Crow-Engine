#pragma once

#include "crow_lib.hpp"

namespace crowe
{
  class Window {
  public:
    Window();

    void DestroyWindow();

    GLFWwindow *GetWindow()
    { return window; }

    void InitializeGLFW();

    void InitializeWindow();

    VkSurfaceKHR CreateVulkanSurface(VkInstance instance);

    void windowLoop();

  private:
    GLFWwindow *window;
  };
}
