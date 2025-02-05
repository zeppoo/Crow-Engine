#pragma once
#include "Vulkan/Vulkan_Types.hpp"
#include <GLFW/glfw3.h>

namespace core
{
  class Window {
  public:
    static Window &GetInstance()
    { return instance; }

    void DestroyWindow();

    GLFWwindow *GetWindow()
    { return window; }

    void InitializeGLFW();

    void InitializeWindow();

    VkSurfaceKHR CreateVulkanSurface(VkInstance instance);

    void windowLoop();

  private:
    Window();

    Window(const Window &) = delete;

    Window &operator=(const Window &) = delete;

    // Members
    static Window instance;
    GLFWwindow *window;
  };
}
