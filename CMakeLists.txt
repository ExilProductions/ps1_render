cmake_minimum_required(VERSION 3.10)

# Project name
project(Ps1Engine)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/renderer)
include_directories(${CMAKE_SOURCE_DIR}/loader)
include_directories(${CMAKE_SOURCE_DIR}/external/stb)

# Source files
set(SOURCES
    ${CMAKE_SOURCE_DIR}/renderer/renderer.cpp
    ${CMAKE_SOURCE_DIR}/loader/mesh.cpp
    ${CMAKE_SOURCE_DIR}/../cube.cpp
)

# Executable
add_executable(Ps1Engine ${SOURCES})

# Link libraries
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(Ps1Engine OpenGL::GL GLEW::GLEW glfw)