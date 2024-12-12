cmake_minimum_required(VERSION 3.14)
project(OpenGLFinalProject VERSION 1.0 LANGUAGES C CXX)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # Ensure static libraries are in bin/

# Find packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Ensure all dependencies are found
if(NOT OpenGL_FOUND)
    message(FATAL_ERROR "OpenGL not found!")
endif()

if(NOT glfw3_FOUND)
    message(FATAL_ERROR "GLFW3 not found!")
endif()

if(NOT glm_FOUND)
    message(FATAL_ERROR "GLM not found!")
endif()

# Find stb_image
find_path(STB_IMAGE_INCLUDE_DIR
    NAMES stb_image.h
    PATHS
        "C:/msys64/mingw64/include"
    PATH_SUFFIXES stb
)

if(NOT STB_IMAGE_INCLUDE_DIR)
    message(FATAL_ERROR "stb_image.h not found")
endif()

# Add glad library
add_library(glad STATIC src/glad.c)
set_target_properties(glad PROPERTIES 
    LINKER_LANGUAGE C
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY} # Place libglad.a in bin/
)
target_include_directories(glad PUBLIC "C:/msys64/mingw64/include")

# Explicitly list source and header files
file(GLOB_RECURSE SRC_FILES "src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "src/*.h")

# Add executable
add_executable(OpenGLFinalProject ${SRC_FILES} ${HEADER_FILES})

# Include directories
target_include_directories(OpenGLFinalProject PRIVATE
    ${OPENGL_INCLUDE_DIR}
    ${GLFW3_INCLUDE_DIR}
    ${GLM_INCLUDE_DIRS}
    ${STB_IMAGE_INCLUDE_DIR}
    "C:/msys64/mingw64/include" # For glad and KHR headers
)

# Link libraries
target_link_libraries(OpenGLFinalProject PRIVATE
    ${OPENGL_gl_LIBRARY}
    glad
    glfw
    glm::glm
)

# Copy shader and texture files to the executable's assets directory
add_custom_command(TARGET OpenGLFinalProject POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/shaders
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/shaders
)

add_custom_command(TARGET OpenGLFinalProject POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/textures
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/textures
)
