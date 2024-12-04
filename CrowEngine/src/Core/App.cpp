#include "Core/App.hpp"
#include "Logger.hpp"
#include "Config/SettingsManager.hpp"
#include "Core/Window.hpp"

namespace core
{
  App App::instance;

  App::App()
  {
    logger::Info("Application Started");
  }

  void App::StartApplication()
  {
      settings::StartRunning();
      logger::Info("Creating Window...");
      window = std::make_unique<Window>();
      logger::Info("Window created succesfully!");
      vulkanModule = std::make_unique<vulkan::VulkanModule>(window);
  }

  void App::RunApplication()
  {
    while (settings::getEngineConfig().isRunning)
    {
      while (!glfwWindowShouldClose(window->GetWindow()))
      {
        glfwPollEvents();
        vulkanModule->RenderFrame();
      }
      break;
    }
  }

  bool App::ShutdownApplication()
  {
    vulkanModule->ShutDown();
    window->DestroyWindow();
    logger::CallShutdown("All Objects Succesfully Destroyed!");
    return true;
  }
}