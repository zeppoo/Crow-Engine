#pragma once
#include "Window.hpp"
#include "../Vulkan/VulkanBackend.hpp"
#include <memory>

namespace crowe
{
class App {
public:
  static App& GetInstance(){return instance;}

  void StartApplication();
  void RunApplication();
  bool ShutdownApplication();

private:
  App();
  App(const App&) = delete;
  App& operator=(const App&) = delete;

  // Members
  static App instance;
  std::unique_ptr<Window> window;
  std::unique_ptr<VulkanModule> vulkanModule;
};
}
