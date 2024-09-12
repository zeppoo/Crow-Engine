#include "App.hpp"
#include "Window.hpp"
#include "core_globals.hpp"

namespace crowe
{
  void App::StartApplication()
  {
    std::string Stages[4] = {"Window", "Vulkan","Engine", "Game"};
    InitializeGLFW();
    InitializeWindow();
    vulkanModule = new VulkanModule();
  }

  void App::RunApplication()
  {
    windowLoop();
  }

  void App::ShutdownApplication()
  {
    glfwDestroyWindow(getWindow());
    glfwTerminate();
    delete vulkanModule;
  }
}