cmake_minimum_required(VERSION 3.10)

# Project Name and Language
project(Raytracer LANGUAGES CXX)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Ensure main.out is placed inside build/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $ENV{binDir})

# Set project root directory (go one level up from cmake/)
set(PROJECT_ROOT ${CMAKE_SOURCE_DIR}/..)

# Get Enviroment Variables
set(SRC_DIR $ENV{srcDir})

# Set executable name
set(EXECUTABLE_NAME $ENV{executableName})
# Collect source files
file(GLOB_RECURSE SRC_FILES
  ${SRC_DIR}/Application/*
  ${SRC_DIR}/core/*
  ${SRC_DIR}/dependencies/*
  #${SRC_DIR}/dependencies/imgui/*
  #${SRC_DIR}/dependencies/imgui/backends/*
  ${SRC_DIR}/* 
)

# Include directories
include_directories(
  ${SRC_DIR}
  ${SRC_DIR}/dependencies/imgui
)

# Define the executable
add_executable(${EXECUTABLE_NAME} ${SRC_FILES})

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(${EXECUTABLE_NAME}
  PRIVATE
  glfw
  OpenGL::GL
)
