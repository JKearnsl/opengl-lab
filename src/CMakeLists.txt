cmake_minimum_required(VERSION 3.29)
project(opengl_lab)

set(CMAKE_CXX_STANDARD 20)

add_executable(opengl_lab main.cpp
        window.cpp
        window.h)
target_link_libraries(opengl_lab OpenGL::GL glfw)
