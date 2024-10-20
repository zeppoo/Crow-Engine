# Set the metadata directory
set(RESOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Use file(GLOB) to gather all .json files in the metadata directory
file(GLOB JSON_FILES "${RESOURCE_DIR}/*.json")
file(GLOB SHADER_FILES "${RESOURCE_DIR}/Shaders/*")
file(GLOB SPV_SHADER_FILES "${RESOURCE_DIR}/Shaders/CompiledShaders/*")

# For debugging, you can print the collected files
message(STATUS "Json files found: ${JSON_FILES}")
message(STATUS "Shader files found: ${SHADER_FILES}")
message(STATUS "SPV Shader files found: ${SPV_SHADER_FILES}")

set(RESOURCE_FILES ${JSON_FILES} ${SHADER_FILES} ${SPV_SHADER_FILES})

# Create a string containing all json files
foreach(FILE_PATH ${RESOURCE_FILES})
    get_filename_component(FILE_NAME ${FILE_PATH} NAME)
    string(REPLACE "." "_" SAFE_FILE_NAME ${FILE_NAME})
    set(PATH_LIST "${PATH_LIST}    inline const char* ${SAFE_FILE_NAME} = \"${FILE_PATH}\";\n")
endforeach()

set(RESOURCE_DIR "${RESOURCE_DIR}/")

# Use configure_file to substitute the paths into the header
configure_file(
        ${RESOURCE_DIR}/ConfigPaths.in.hpp
        ${PROJECT_BINARY_DIR}/resources/ConfigPaths.hpp
)