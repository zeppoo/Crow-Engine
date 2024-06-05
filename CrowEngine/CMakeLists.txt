cmake_minimum_required(VERSION 3.28)

project(CrowEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set Source Files
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

# Add scripts
add_executable(CrowEngine ${SOURCE_FILES})

# Find Libraries
add_subdirectory ("external/glfw")
add_subdirectory ("external/glm")
add_subdirectory("external/imgui")

# Find Vulkan package
find_package(Vulkan REQUIRED)

# Include libraries in Project
target_include_directories(CrowEngine PRIVATE
        external/glfw/include
        external/glm
        external/imgui
        ${Vulkan_INCLUDE_DIRS}
)

# Link libraries to Project
target_link_libraries(CrowEngine PRIVATE glfw glm imgui Vulkan::Vulkan)


# Give imgui access to libraries
target_include_directories(imgui PRIVATE ${Vulkan_INCLUDE_DIRS})
target_include_directories(imgui PRIVATE ${GLFW_INCLUDE_DIRS})
target_link_libraries(imgui PRIVATE ${Vulkan_LIBRARIES})
target_link_libraries(imgui PRIVATE glfw)


