#include <cmath>
#include <GL/gl.h>

#include "window.h"


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

void set_material_color(unsigned int rgba) {
    GLfloat material_color[] = {
        static_cast<float>((rgba >> 24) & 0xff) / 255.0f,
        static_cast<float>((rgba >> 16) & 0xff) / 255.0f,
        static_cast<float>((rgba >> 8) & 0xff) / 255.0f,
        static_cast<float>(rgba & 0xff) / 255.0f
    };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material_color);
}

void draw_circle(
    float x,
    float y,
    float r,
    unsigned int fill_rgba = 0xff000000,
    unsigned int border_rgba = 0x000000FF,
    unsigned int border_width = 5
) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    unsigned int border_radius = r - border_width;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_TRIANGLE_FAN);

    set_material_color(fill_rgba);

    // Draw inner circle
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0, 0);
    for (int i = 0; i <= 360; i++) {
        float angle = 3.1415926f * i / 180;
        glVertex2f(r * cos(angle), r * sin(angle));
    }
    glEnd();

    // Draw border circle
    glBegin(GL_TRIANGLE_STRIP);
    set_material_color(border_rgba);
    for (int i = 0; i <= 360; i++) {
        float angle = 3.1415926f * i / 180;
        glVertex2f(border_radius * cos(angle), border_radius * sin(angle));
        glVertex2f(r * cos(angle), r * sin(angle));
    }
    glEnd();

    glPopMatrix();
}


int main() {
    const Window window("Lab 2", 800, 600);
    setup_lighting();
    window.loop([] {
        draw_circle(415, 400, 50, 0xffffff00, 0xF29E00FF);
        draw_circle(540, 400, 50, 0xffffff00, 0x009E24FF);

        draw_circle(350, 450, 50, 0xffffff00, 0x00A2C5FF);
        draw_circle(475, 450, 50, 0xffffff00, 0x010101FF);
        draw_circle(600, 450, 50, 0xffffff00, 0xFF0000FF);
    });

    return 0;
}
