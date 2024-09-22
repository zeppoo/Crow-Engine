# ImGui source files
file(GLOB IMGUI_SOURCE_FILES "*.cpp")

# ImGui backend for Vulkan
file(GLOB IMGUI_BACKEND_VULKAN_SOURCE_FILES "backends/imgui_impl_vulkan.cpp")

# ImGui backend for GLFW
file(GLOB IMGUI_BACKEND_GLFW_SOURCE_FILES "backends/imgui_impl_glfw.cpp")

# Combine all source files
set(IMGUI_ALL_SOURCE_FILES ${IMGUI_SOURCE_FILES} ${IMGUI_BACKEND_VULKAN_SOURCE_FILES} ${IMGUI_BACKEND_GLFW_SOURCE_FILES})

# Create a library target for ImGui
add_library(imgui ${IMGUI_ALL_SOURCE_FILES})

# Add include directories for ImGui headers
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
