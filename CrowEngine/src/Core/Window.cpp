#include "Window.hpp"

namespace crowe
{
  Window::Window()
  {
    InitializeGLFW();
    InitializeWindow();
  }

  Window::~Window()
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
    window = glfwCreateWindow(settings.windowConfig.width, settings.windowConfig.height, settings.engineConfig.engineName.c_str(), nullptr, nullptr);
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
  }

}
