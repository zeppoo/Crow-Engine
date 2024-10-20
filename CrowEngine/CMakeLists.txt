cmake_minimum_required(VERSION 3.28)

project(CrowEngine VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(LIBS_DIR ${CMAKE_SOURCE_DIR}/libs)
set(SOURCES_DIR ${CMAKE_SOURCE_DIR}/src)
set(RESOURCE_DIR ${CMAKE_SOURCE_DIR}/resources)
set(CPPR ${LIBS_DIR}/cppr/libCPPR.a)

# Link Source Files
file(GLOB_RECURSE SOURCE_FILES ${SOURCES_DIR}/*.cpp)
add_executable(CrowEngine ${SOURCE_FILES})

# Set External Libraries
add_subdirectory(${EXTERNAL_DIR}/glm)
add_subdirectory(${EXTERNAL_DIR}/glfw)
add_subdirectory(${EXTERNAL_DIR}/imgui)
add_subdirectory(${INCLUDE_DIR})
add_subdirectory(${RESOURCE_DIR})

# Find Packages
find_package(Vulkan REQUIRED)

# Include directories for external libraries and Vulkan
target_include_directories(CrowEngine PRIVATE
        external/glfw/include
        external/glm
        external/imgui
        include
        libs/cppr
        ${PROJECT_BINARY_DIR}/resources
        ${Vulkan_INCLUDE_DIRS}
)

# Link the libraries
target_link_libraries(CrowEngine PRIVATE glfw glm imgui Vulkan::Vulkan ${CPPR})

# Ensure ImGui has access to Vulkan and GLFW libraries
target_include_directories(imgui PRIVATE
        ${Vulkan_INCLUDE_DIRS}
        ${GLFW_INCLUDE_DIRS})
target_link_libraries(imgui PRIVATE
        ${Vulkan_LIBRARIES}
        glfw)
