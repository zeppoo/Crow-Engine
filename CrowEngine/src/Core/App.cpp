#include "App.hpp"
#include "../Logger.hpp"
#include "Window.hpp"


namespace crowe
{

  void App::StartApplication()
  {
    window = std::make_unique<Window>();
    logger.Log("Window created succesfully!", INFO);
    vulkanModule = std::make_unique<VulkanModule>(window);
  }

  void App::RunApplication()
  {
    window->windowLoop();
  }

  void App::ShutdownApplication()
  {

  }
}