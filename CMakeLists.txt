cmake_minimum_required(VERSION 3.14)
project(OpenGLFinalProject VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

if(NOT OpenGL_FOUND)
    message(FATAL_ERROR "OpenGL no encontrado!")
endif()

if(NOT glfw3_FOUND)
    message(FATAL_ERROR "GLFW3 no encontrado!")
endif()

if(NOT glm_FOUND)
    message(FATAL_ERROR "GLM no encontrado!")
endif()

# Locate stb_image.h
find_path(STB_IMAGE_INCLUDE_DIR
    NAMES stb_image.h
    PATHS 
        "C:/msys64/mingw64/include"
        "C:/msys64/mingw64/include/stb"
    PATH_SUFFIXES "stb"
)

if(NOT STB_IMAGE_INCLUDE_DIR)
    message(FATAL_ERROR "stb_image.h not found. Please verify its installation.")
else()
    message(STATUS "Found stb_image.h in ${STB_IMAGE_INCLUDE_DIR}")
endif()

add_library(glad STATIC src/glad.c)
set_target_properties(glad PROPERTIES 
    LINKER_LANGUAGE C
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
)
target_include_directories(glad PUBLIC "C:/msys64/mingw64/include")

# Lista de todos los .cpp excepto Untitled-1.cpp
set(SRC_FILES
    src/main.cpp
    src/camera.cpp
    src/light.cpp
    src/lighthouse.cpp
    src/mesh.cpp
    src/plane.cpp
    src/scene.cpp
    src/shader.cpp
    src/texture.cpp
    src/constants.h
    src/camera.h
    src/light.h
    src/lighthouse.h
    src/mesh.h
    src/plane.h
    src/scene.h
    src/shader.h
    src/texture.h
    src/Constants.h
)

add_executable(OpenGLFinalProject ${SRC_FILES})

target_include_directories(OpenGLFinalProject PRIVATE
    ${OPENGL_INCLUDE_DIR}
    ${GLFW3_INCLUDE_DIR}
    ${GLM_INCLUDE_DIRS}
    ${STB_IMAGE_INCLUDE_DIR}
    "C:/msys64/mingw64/include"
)

target_link_libraries(OpenGLFinalProject PRIVATE
    ${OPENGL_gl_LIBRARY}
    glad
    glfw
    glm::glm
)

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
