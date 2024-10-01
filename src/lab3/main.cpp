#include <cmath>
#include <GL/gl.h>
#include "window.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>


GLuint load_texture(const char* filename) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);
    if (!data) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        exit(-1);
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return texture;
}

void draw_textured_ring(
        float x,
        float y,
        float z,
        float r,
        float dr,
        float depth = 10,
        GLuint texture = 0
    ) {
    glPushMatrix();
    glTranslatef(x, y, z);

    float ring_inner_radius = r;
    float ring_outer_radius = r + dr;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    for (int i = 0; i < 360; i++) {
        float angle1 = (M_PI * i / 180.0f);
        float angle2 = (M_PI * (i + 1) / 180.0f);

        // Внешняя часть кольца
        glBegin(GL_QUADS);
        // Нижняя грань
        glTexCoord2f(0.0f, 0.0f); glVertex3f(ring_outer_radius * cos(angle1), ring_outer_radius * sin(angle1), 0);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(ring_outer_radius * cos(angle2), ring_outer_radius * sin(angle2), 0);
        // Верхняя грань
        glTexCoord2f(1.0f, 1.0f); glVertex3f(ring_outer_radius * cos(angle2), ring_outer_radius * sin(angle2), depth);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(ring_outer_radius * cos(angle1), ring_outer_radius * sin(angle1), depth);
        glEnd();

        // Внутренняя часть кольца
        glBegin(GL_QUADS);
        // Нижняя грань
        glTexCoord2f(0.0f, 0.0f); glVertex3f(ring_inner_radius * cos(angle1), ring_inner_radius * sin(angle1), 0);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(ring_inner_radius * cos(angle2), ring_inner_radius * sin(angle2), 0);
        // Верхняя грань
        glTexCoord2f(1.0f, 1.0f); glVertex3f(ring_inner_radius * cos(angle2), ring_inner_radius * sin(angle2), depth);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(ring_inner_radius * cos(angle1), ring_inner_radius * sin(angle1), depth);
        glEnd();

        // Стороны между внутренним и внешним кольцом
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(ring_inner_radius * cos(angle1), ring_inner_radius * sin(angle1), 0);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(ring_inner_radius * cos(angle2), ring_inner_radius * sin(angle2), 0);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(ring_outer_radius * cos(angle2), ring_outer_radius * sin(angle2), 0);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(ring_outer_radius * cos(angle1), ring_outer_radius * sin(angle1), 0);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(ring_inner_radius * cos(angle1), ring_inner_radius * sin(angle1), depth);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(ring_inner_radius * cos(angle2), ring_inner_radius * sin(angle2), depth);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(ring_outer_radius * cos(angle2), ring_outer_radius * sin(angle2), depth);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(ring_outer_radius * cos(angle1), ring_outer_radius * sin(angle1), depth);
        glEnd();
    }

    glDisable(GL_TEXTURE_2D); // Отключите текстурирование
    glPopMatrix();
}


int main() {
    const Window window("Lab 3", 800, 600);
    Window::setup_lighting();

    static GLuint texture = load_texture("src/lab3/texture.jpg");

    window.loop([](GLFWwindow*) {
        draw_textured_ring(-30, -10, 0, 10, 3, 4, texture);
        draw_textured_ring(0, -10, 0, 10, 3, 4, texture);
        draw_textured_ring(30, -10, 0, 10, 3, 4, texture);

        draw_textured_ring(-15, -20, 0, 10, 3, 4, texture);
        draw_textured_ring(15, -20, 0, 10, 3, 4, texture);
    });

    return 0;
}