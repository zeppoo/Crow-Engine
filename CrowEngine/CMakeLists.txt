cmake_minimum_required(VERSION 3.28)

project(CrowEngine VERSION 1.0)
project(PrintTree VERSION 1.0)
add_executable(PrintTree PrintPathTree.cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find Vulkan package
find_package(Vulkan REQUIRED)

# Add external dependencies
add_subdirectory(external/glfw)
add_subdirectory(external/glm)
add_subdirectory(external/imgui)

# Collect all source files
file(GLOB_RECURSE SOURCES
        "src/*.cpp"
)

file(GLOB_RECURSE HEADERS
        "include/*.hpp"
)

# Set include directories
set(INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/cppr
        ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/include
        ${CMAKE_CURRENT_SOURCE_DIR}/external/glm
        ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/external/tinyObjLoader
        ${Vulkan_INCLUDE_DIRS}
)

# Create executable
add_executable(CrowEngine ${SOURCES} ${HEADERS}
        src/Renderer/UBO_Manager.cpp
        src/Renderer/UBO_Manager.hpp)

# Link include directories to target
target_include_directories(CrowEngine PRIVATE ${INCLUDE_DIRS})

# Add resources directory
add_subdirectory(resources)

# Link libraries
target_link_libraries(CrowEngine PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/cppr/libCPPR.a
        glfw
        glm
        imgui
        tinyObjLoader
        Vulkan::Vulkan
)

# Copy shader files to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/Shaders
        DESTINATION ${CMAKE_BINARY_DIR})

# Configure resource paths
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/ConfigPaths.in.hpp
        ${CMAKE_BINARY_DIR}/ConfigPaths.hpp
)
target_include_directories(CrowEngine PRIVATE ${CMAKE_BINARY_DIR})