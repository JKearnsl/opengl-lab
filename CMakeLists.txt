cmake_minimum_required(VERSION 3.29)
project(opengl_lab)

set(CMAKE_CXX_STANDARD 20)

# GLFW - window and input handling
add_subdirectory(extern/glfw)

find_package(OpenGL REQUIRED)

add_subdirectory(src/lab1)
add_subdirectory(src/lab2)
