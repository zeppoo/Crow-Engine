#include "Window.hpp"

namespace crowe
{

GLFWwindow* window;
WinProps win_props = {1000, 800, "Crow Engine"};
GLFWwindow *getWindow() { return window; }

void InitializeGLFW()
{
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

void InitializeWindow()
{
  window = glfwCreateWindow(win_props.width, win_props.height, win_props.name.c_str(), nullptr, nullptr);
  if (!window)
  {
    std::cout << "Failed to create GLFW window\n";
    glfwTerminate();
    return;
  }
}

VkSurfaceKHR CreateVulkanSurface(VkInstance instance)
{
  VkSurfaceKHR surface;

  if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
  {
    throw std::runtime_error("Failed to create window surface");
  }

  return surface;
}

void windowLoop()
{
  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
  }


}
}
