#include "Gui/GUI_Module.hpp"

namespace GUI
{
  void GUIModule::InitializeImGUI()
   {
     // Create ImGui context
     ImGui::CreateContext();

     // Set default style
     ImGui::StyleColorsDark();

     // Set configuration flags
     ImGuiIO& io = ImGui::GetIO();
     io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls

     // Initialize platform-specific bindings
     ImGui_ImplGlfw_InitForVulkan(window->GetWindow(), true); // GLFW binding
   }

  void GUIModule::ImplementVulkan(std::unique_ptr<vulkan::VulkanModule> vulkanModule)
  {
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = vulkanModule->GetDevice()->GetVkInstance(); // Your Vulkan instance
    init_info.PhysicalDevice = vulkanModule->GetDevice()->GetPhysicDevice(); // Your physical device
    init_info.Device = vulkanModule->GetDevice()->GetDevice(); // Your logical device
    init_info.QueueFamily = vulkanModule->GetQueueManager()->GetPresentQueues()[0].familyIndex; // Graphics queue family index
    init_info.Queue = *vulkanModule->GetQueueManager()->GetPresentQueues()[0].pQueue; // Your Vulkan graphics queue
    init_info.PipelineCache = VK_NULL_HANDLE; // Optional, can pass null
    init_info.DescriptorPool = imguiDescriptorPool;
    init_info.MinImageCount = vulkan::FRAMES_IN_FLIGHT; // Swapchain image count
    init_info.ImageCount = vulkan::FRAMES_IN_FLIGHT; // Swapchain image count
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT; // Change if using MSAA
    init_info.Allocator = nullptr; // Custom allocator, pass null if unused
  }



}