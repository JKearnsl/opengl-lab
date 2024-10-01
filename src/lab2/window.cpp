//
// Created by jkearnsl on 08.09.2024.
//

#include "window.h"

#include <cmath>
#include <iostream>
#include <GL/glu.h>


float camera_angle = 0.0f;      // Угол
float camera_speed = 0.01f;     // Скорость
float camera_radius = 200.0f;   // Радиус

void setup_camera() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float camX = camera_radius * sin(camera_angle);
    float camZ = camera_radius * cos(camera_angle);

    // Параметры камеры
    gluLookAt(camX, 0.0, camZ,      // Положение камеры
              0.0, 0.0, 0.0,    // Камера смотрит на центр сцены
              0.0, 1.0, 0.0);     // Вектор "вверх"
}

void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        camera_angle -= camera_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        camera_radius -= 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        camera_radius += 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        camera_angle += camera_speed;
    }
}

Window::Window(const char* title, const int width, const int height) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(-1);
    }

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    framebuffer_size_callback(window, framebufferWidth, framebufferHeight);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::loop(void (*renderFunc)(GLFWwindow* window)) const {
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        process_input(window);
        setup_camera();

        // Фиксация источника света
        GLfloat light_position[] = { 0.0f, 0.0f, 100.0f, 1.0f };
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);

        renderFunc(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Window::framebuffer_size_callback(GLFWwindow* window, const int width, const int height) {
    glViewport(0, 0, width, height);

    if (height == 0) return;

    const float fovY = 45.0f;  // Фиксированный угол поля зрения по вертикали
    float aspect = static_cast<float>(width) / static_cast<float>(height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovY, aspect, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

void Window::setup_lighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    // GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    // glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}
