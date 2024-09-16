#include "Window.hpp"
#include "../config/SettingsManager.hpp"
#include "../Logger.hpp"

namespace crowe
{
  Window::Window()
  {
    InitializeGLFW();
    InitializeWindow();
  }

  void Window::DestroyWindow()
  {
    glfwDestroyWindow(window);
    glfwTerminate();
  }

  void Window::InitializeGLFW()
  {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  }

  void Window::InitializeWindow()
  {
    window = glfwCreateWindow(getWindowConfig().width, getWindowConfig().height, getEngineConfig().engineName.c_str(), nullptr, nullptr);
    if (!window)
    {
      std::cout << "Failed to create GLFW window\n";
      glfwTerminate();
      return;
    }
  }

  VkSurfaceKHR Window::CreateVulkanSurface(VkInstance instance)
  {
    VkSurfaceKHR surface;

    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
    {
      throw std::runtime_error("Failed to create window surface");
    }
    return surface;
  }

  void Window::windowLoop()
  {
    while (!glfwWindowShouldClose(window))
    {
      glfwPollEvents();
    }
    SHUTDOWN_APP("Window Closed");
  }
}
