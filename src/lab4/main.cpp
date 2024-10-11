#include <cmath>
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

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void draw_cube(
        float x,
        float y,
        float z,
        float width,
        float height,
        float depth
    ) {
    glPushMatrix();
    glTranslatef(x, y, z);

    glBegin(GL_QUADS);

    // Front face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(width, 0, 0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(width, height, 0);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0, height, 0);

    // Back face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, -depth);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(width, 0, -depth);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(width, height, -depth);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0, height, -depth);

    // Top face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0, height, 0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(width, height, 0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(width, height, -depth);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0, height, -depth);

    // Bottom face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(width, 0, 0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(width, 0, -depth);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 0, -depth);

    // Right face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(width, 0, 0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(width, height, 0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(width, height, -depth);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(width, 0, -depth);

    // Left face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0, height, 0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0, height, -depth);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 0, -depth);

    glEnd();
    glPopMatrix();
}


int main() {
    const Window window("Lab 4", 800, 600);
    Window::setup_lighting();

    static GLuint texture_yellow = load_texture("src/lab4/texture.jpg");
    static GLuint texture_black = load_texture("src/lab4/texture_black.jpeg");
    static GLuint texture_blue = load_texture("src/lab4/texture_blue.jpg");
    static GLuint texture_green = load_texture("src/lab4/texture_green.jpg");
    static GLuint texture_red = load_texture("src/lab4/texture_red.jpeg");


    static float rings[5][3] = {
        // x, y, z
        {-30, -10, 0},
        {-30, -10, 0},
        {-30, -10, 0},
        {-30, -20, 0},
        {-30, -20, 0}
    };

    static GLuint textures[5] = {
        texture_blue,
        texture_yellow,
        texture_black,
        texture_green,
        texture_red
    };

    static float target_x[5] = {-30.0f, 0.0f, 30.0f, -15.0f, 15.0f};
    static bool state[5] = {false, false, false, false, false};
    static float speed = 0.5f;
    static int step = 0;

    window.loop([](GLFWwindow*) {
        draw_cube(-50, -50, 10, 10, 10, 10);

        for (int i = 0; i < 5; i++) {
            if (!state[i]) {
                state[i] = true;
            }

            if (state[i]) {
                if (rings[i][0] < target_x[i]) {
                    rings[i][0] += speed;
                } else {
                    state[i] = false;
                }
            }

            draw_textured_ring(rings[i][0], rings[i][1], rings[i][2], 10, 3, 4, textures[i]);
        }

        if (step < 5 && !state[step]) {
            state[step] = true;
            step++;
        }
    });

    return 0;
}
