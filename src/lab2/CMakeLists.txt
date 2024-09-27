cmake_minimum_required(VERSION 3.29)
project(opengl_lab)

set(CMAKE_CXX_STANDARD 20)

find_package(GLUT REQUIRED)

add_executable(lab2 main.cpp window.cpp window.h)

target_link_libraries(lab2 OpenGL::GL glfw glut)
