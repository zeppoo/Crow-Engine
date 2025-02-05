#include "Core/Entry.hpp"
#include "Engine/Engine.hpp"
#include "Renderer/Renderer.hpp"
#include "Utils/Logger.hpp"
#include "Utils/GenerateDefaults.hpp"
#include "Vulkan/Vulkan_Module.hpp"
#include "Gui/GUI_Module.hpp"
#include "Core/Window.hpp"
#include "Core/App.hpp"

void Initialize_Resources()
{
  GenerateDefaultPipelineConfig();
  GenerateDefaultRenderPassConfig();
}

void Initialize_SubSystems()
{
  logger::Logger::GetInstance().InitializeLogger();
  core::Window::GetInstance().InitializeGLFW();
  core::Window::GetInstance().InitializeWindow();
  vulkan::VulkanModule::GetInstance().InitiliazeVulkan();
}

void Initialize_Application()
{

}

void Initialize_Shutdown()
{

}