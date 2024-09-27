#include <cmath>
#include <GL/gl.h>
#include "window.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

void setup_lighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}

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

void set_material_color(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
    GLfloat material_color[] = { r, g, b, a };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material_color);
}

void draw_textured_ring(
    float x,
    float y,
    float r,
    unsigned int border_width = 5,
    GLuint texture = 0
) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    unsigned int border_radius = r - border_width;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Draw border circle
    glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2f(0.5f, 0.5f);
    glVertex2f(0, 0);
    set_material_color(1.0f, 1.0f, 1.0f, 1.0f);

    for (int i = 0; i <= 360; i++) {
        float angle = 3.1415926f * i / 180;
        float tx = 0.5f + 0.5f * cos(angle);
        float ty = 0.5f + 0.5f * sin(angle);
        glTexCoord2f(tx, ty);
        glVertex2f(border_radius * cos(angle), border_radius * sin(angle));
        glVertex2f(r * cos(angle), r * sin(angle));
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

int main() {
    const Window window("Lab 3", 800, 600);
    setup_lighting();
    static GLuint texture = load_texture("src/lab3/texture.jpg");

    window.loop([] {
        draw_textured_ring(415, 400, 50, 15, texture);
        draw_textured_ring(540, 400, 50, 15, texture);

        draw_textured_ring(350, 450, 50, 15, texture);
        draw_textured_ring(475, 450, 50, 15, texture);
        draw_textured_ring(600, 450, 50, 15, texture);
    });

    return 0;
}