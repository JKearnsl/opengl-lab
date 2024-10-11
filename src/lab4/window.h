//
// Created by jkearnsl on 08.09.2024.
//

#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>

class Window {
public:
    Window(const char* title, int width, int height);
    ~Window();

    void loop(void (*renderFunc)(GLFWwindow* window)) const;

    static void setup_lighting();

private:
    GLFWwindow* window;

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};
#endif //WINDOW_H
