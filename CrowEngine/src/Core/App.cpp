#include "App.hpp"
#include "Window.hpp"
#include "core_globals.hpp"

namespace crowe
{
  void App::StartApplication()
  {
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