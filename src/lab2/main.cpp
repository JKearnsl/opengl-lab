#include <cmath>
#include <GL/gl.h>

#include "window.h"


void set_material_color(unsigned int rgba) {
    GLfloat material_color[] = {
        static_cast<float>((rgba >> 24) & 0xff) / 255.0f,
        static_cast<float>((rgba >> 16) & 0xff) / 255.0f,
        static_cast<float>((rgba >> 8) & 0xff) / 255.0f,
        static_cast<float>(rgba & 0xff) / 255.0f
    };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material_color);
}


void draw_ring(
        float x,
        float y,
        float z,
        float r,
        float dr,
        float depth = 10,
        unsigned int color_rgba = 0x000000FF
    ) {
    glPushMatrix();
    glTranslatef(x, y, z);

    float ring_inner_radius = r;
    float ring_outer_radius = r + dr;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    set_material_color(color_rgba);

    for (int i = 0; i < 360; i++) {
        float angle1 = (M_PI * i / 180.0f);
        float angle2 = (M_PI * (i + 1) / 180.0f);

        // Внешняя часть кольца
        glBegin(GL_QUADS);
        // Нижняя грань
        glVertex3f(ring_outer_radius * cos(angle1), ring_outer_radius * sin(angle1), 0);
        glVertex3f(ring_outer_radius * cos(angle2), ring_outer_radius * sin(angle2), 0);
        // Верхняя грань
        glVertex3f(ring_outer_radius * cos(angle2), ring_outer_radius * sin(angle2), depth);
        glVertex3f(ring_outer_radius * cos(angle1), ring_outer_radius * sin(angle1), depth);
        glEnd();

        // Внутренняя часть кольца
        glBegin(GL_QUADS);
        // Нижняя грань
        glVertex3f(ring_inner_radius * cos(angle1), ring_inner_radius * sin(angle1), 0);
        glVertex3f(ring_inner_radius * cos(angle2), ring_inner_radius * sin(angle2), 0);
        // Верхняя грань
        glVertex3f(ring_inner_radius * cos(angle2), ring_inner_radius * sin(angle2), depth);
        glVertex3f(ring_inner_radius * cos(angle1), ring_inner_radius * sin(angle1), depth);
        glEnd();

        // Стороны между внутренним и внешним кольцом
        glBegin(GL_QUADS);

        glVertex3f(ring_inner_radius * cos(angle1), ring_inner_radius * sin(angle1), 0);
        glVertex3f(ring_inner_radius * cos(angle2), ring_inner_radius * sin(angle2), 0);
        glVertex3f(ring_outer_radius * cos(angle2), ring_outer_radius * sin(angle2), 0);
        glVertex3f(ring_outer_radius * cos(angle1), ring_outer_radius * sin(angle1), 0);

        glVertex3f(ring_inner_radius * cos(angle1), ring_inner_radius * sin(angle1), depth);
        glVertex3f(ring_inner_radius * cos(angle2), ring_inner_radius * sin(angle2), depth);
        glVertex3f(ring_outer_radius * cos(angle2), ring_outer_radius * sin(angle2), depth);
        glVertex3f(ring_outer_radius * cos(angle1), ring_outer_radius * sin(angle1), depth);
        glEnd();
    }

    glPopMatrix();
}


int main() {
    const Window window("Lab 2", 800, 600);
    Window::setup_lighting();

    window.loop([](GLFWwindow*) {

        draw_ring(-30, -10, 0, 10, 3, 4, 0x005BBFff);
        draw_ring(0, -10, 0, 10, 3, 4, 0x000000FF);
        draw_ring(30, -10, 0, 10, 3, 4, 0xFF0000FF);

        draw_ring(-15, -20, 0, 10, 3, 4, 0xF29E00FF);
        draw_ring(15, -20, 0, 10, 3, 4, 0x009E24FF);

    });

    return 0;
}
