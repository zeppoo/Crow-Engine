#include "App.hpp"
#include "Window.hpp"
#include "../Vulkan/VulkanBackend.hpp"

namespace crowe
{
  void App::StartApplication()
  {
    std::string Stages[4] = {"Window", "Vulkan","Engine", "Game"};
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
    glfwDestroyWindow(getWindow());
    glfwTerminate();
  }
}