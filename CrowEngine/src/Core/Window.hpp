#pragma once

#include "../crow_lib.hpp"

namespace crowe
{
struct WinProps {
  uint32_t width;
  uint32_t height;
  const std::string name;
};

GLFWwindow* getWindow();

void InitializeGLFW();
void InitializeWindow();
VkSurfaceKHR CreateVulkanSurface(VkInstance instance);
static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
void windowLoop();
}
