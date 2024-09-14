#pragma once
#include "Window.hpp"
#include "../Vulkan/VulkanBackend.hpp"
#include <memory>

namespace crowe
{
class App {
public:
  void StartApplication();
  void RunApplication();
  void ShutdownApplication();

private:
  // References
  Logger& logger = Logger::GetInstance();
  // Members
  std::unique_ptr<Window> window;
  std::unique_ptr<VulkanModule> vulkanModule;
};
}
