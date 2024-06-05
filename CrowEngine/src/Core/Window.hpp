#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "../crow_lib.hpp"

namespace crowe
{
struct WinProps {
  uint32_t width;
  uint32_t height;
  const std::string name;
};

GLFWwindow* GetWindow();

void InitializeGLFW();
void InitializeWindow();
VkSurfaceKHR CreateVulkanSurface(VkInstance instance);
static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
void windowLoop();

bool framebufferResized;

}

#endif //WINDOW_HPP
