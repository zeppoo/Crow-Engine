#include "Core/App.hpp"
#include "Logger.hpp"
#include "Config/SettingsManager.hpp"
#include "Core/Window.hpp"

namespace core
{
  App App::instance;

  App::App()
  {
    log::Info("Application Started");
  }

  void App::StartApplication()
  {
      settings::StartRunning();
      log::Info("Creating Window...");
      window = std::make_unique<Window>();
      log::Info("Window created succesfully!");
      vulkanModule = std::make_unique<vulkan::VulkanModule>(window);
  }

  void App::RunApplication()
  {
    while (settings::getEngineConfig().isRunning)
    {
      window->windowLoop();
      break;
    }
  }

  bool App::ShutdownApplication()
  {
    vulkanModule->ShutDown();
    window->DestroyWindow();
    log::CallShutdown("All Objects Succesfully Destroyed!");
    return true;
  }
}