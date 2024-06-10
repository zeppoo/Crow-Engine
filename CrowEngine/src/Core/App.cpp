#include "App.hpp"
#include "Window.hpp"
#include "../Vulkan/VulkanBackend.hpp"

namespace crowe
{
  void App::StartApplication()
  {
    InitializeGLFW();
    InitializeWindow();
    VulkanStartup();
  }

  void App::RunApplication()
  {
    windowLoop();
  }

  void App::ShutdownApplication()
  {
    glfwDestroyWindow(GetWindow());
    glfwTerminate();
  }
}