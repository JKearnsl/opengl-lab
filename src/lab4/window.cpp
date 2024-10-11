//
// Created by jkearnsl on 08.09.2024.
//

#include "window.h"

#include <cmath>
#include <iostream>


float camera_angle = 0.0f;
float camera_speed = 0.01f;
float camera_radius = 200.0f;

float camX = 0.0f;
float camY = 0.0f;
float camZ = 0.0f;

float light_offset_x = 0.0f;
float light_offset_y = 50.0f;
float light_offset_z = 0.0f;

void setup_camera() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    camX = camera_radius * sin(camera_angle);
    camZ = camera_radius * cos(camera_angle);


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
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        exit(-1);
    }

    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    framebuffer_size_callback(window, framebufferWidth, framebufferHeight);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void shadow_projection_matrix(GLfloat *mat, GLfloat light_pos[4], GLfloat plane[4]) {
    GLfloat dot = plane[0] * light_pos[0] +
                  plane[1] * light_pos[1] +
                  plane[2] * light_pos[2] +
                  plane[3] * light_pos[3];

    mat[0]  = dot - light_pos[0] * plane[0];
    mat[4]  = 0.0f - light_pos[0] * plane[1];
    mat[8]  = 0.0f - light_pos[0] * plane[2];
    mat[12] = 0.0f - light_pos[0] * plane[3];

    mat[1]  = 0.0f - light_pos[1] * plane[0];
    mat[5]  = dot - light_pos[1] * plane[1];
    mat[9]  = 0.0f - light_pos[1] * plane[2];
    mat[13] = 0.0f - light_pos[1] * plane[3];

    mat[2]  = 0.0f - light_pos[2] * plane[0];
    mat[6]  = 0.0f - light_pos[2] * plane[1];
    mat[10] = dot - light_pos[2] * plane[2];
    mat[14] = 0.0f - light_pos[2] * plane[3];

    mat[3]  = 0.0f - light_pos[3] * plane[0];
    mat[7]  = 0.0f - light_pos[3] * plane[1];
    mat[11] = 0.0f - light_pos[3] * plane[2];
    mat[15] = dot - light_pos[3] * plane[3];
}

void Window::loop(void (*renderFunc)(GLFWwindow* window)) const {
    GLfloat light_position[] = { 50.0f, 50.0f, 50.0f, 1.0f };

    while (!glfwWindowShouldClose(window)) {

        light_position[0] = 100.0f * sin(glfwGetTime());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        process_input(window);
        setup_camera();

        // GLfloat light_position[] = { camX + light_offset_x, light_offset_y, camZ + light_offset_z, 1.0f };
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);

        renderFunc(window);

        glPushAttrib(GL_ENABLE_BIT);

        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);

        GLfloat shadow_matrix[16];
        GLfloat plane[4] = {0.0f, 0.0f, 1.0f, 0.0f};
        // Уравнение плоскости в пространстве: 0x + 0y + 1z + 0 = 0
        shadow_projection_matrix(shadow_matrix, light_position, plane);

        glPushMatrix();
        glMultMatrixf(shadow_matrix);
        glColor3f(0.0f, 0.0f, 0.0f);
        renderFunc(window);
        glPopMatrix();

        glPopAttrib();
        glEnable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);

        // Light rect
        glPushMatrix();
        glTranslatef(light_position[0], light_position[1], light_position[2]);
        GLfloat material_color[] = {255.0f, 255.0f, 1.0f, 255.0f};
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material_color);
        glBegin(GL_QUADS);
        glVertex3f(0, 0, 0);
        glVertex3f(10, 0, 0);
        glVertex3f(10, 10, 0);
        glVertex3f(0, 10, 0);
        glEnd();
        glPopMatrix();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}


void Window::framebuffer_size_callback(GLFWwindow* window, const int width, const int height) {
    glViewport(0, 0, width, height);

    if (height == 0) return;

    const float fovY = 45.0f;
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

    GLfloat mat_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
}
