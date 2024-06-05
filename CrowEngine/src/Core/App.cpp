#include "App.hpp"

#include "Window.hpp"

namespace crowe
{
  void App::StartApplication()
  {
    InitializeGLFW();
    InitializeWindow();
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