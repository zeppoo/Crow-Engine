#include "../../include/Core/App.hpp"
#include "../../include/General/Logger.hpp"
#include "../../include/Config/SettingsManager.hpp"
#include "../../include/Core/Window.hpp"

namespace crowe
{
  App App::instance;

  App::App()
  {
    INFO("Application Started");
  }

  void App::StartApplication()
  {
      StartRunning();
      INFO("Creating Window...");
      window = std::make_unique<Window>();
      INFO("Window created succesfully!");
      vulkanModule = std::make_unique<VulkanModule>(window);
  }

  void App::RunApplication()
  {
    while (getEngineConfig().isRunning)
    {
      window->windowLoop();
      break;
    }
  }

  bool App::ShutdownApplication()
  {
    vulkanModule->VulkanShutDown();
    window->DestroyWindow();
    SHUTDOWN_APP("All Objects Succesfully Destroyed!");
    return true;
  }
}