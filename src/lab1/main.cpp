
#include "window.h"

void gl_rect_with_border(
    float x,
    float y,
    float w,
    float h,
    unsigned int fill_rgba = 0xFF000000,
    unsigned int border_rgba = 0x000000FF
    ) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);

    glBegin(GL_QUADS);
    glColor4f(
        static_cast<float>((fill_rgba >> 24) & 0xff) / 255.0f,
        static_cast<float>((fill_rgba >> 16) & 0xff) / 255.0f,
        static_cast<float>((fill_rgba >> 8) & 0xff) / 255.0f,
        static_cast<float>(fill_rgba & 0xff) / 255.0f
    );
    glVertex2f(0, 0);
    glVertex2f(0, h);
    glVertex2f(w, h);
    glVertex2f(w, 0);
    glEnd();

    // border
    glBegin(GL_LINE_LOOP);
    glColor4f(
        static_cast<float>((border_rgba >> 24) & 0xff) / 255.0f,
        static_cast<float>((border_rgba >> 16) & 0xff) / 255.0f,
        static_cast<float>((border_rgba >> 8) & 0xff) / 255.0f,
        static_cast<float>(border_rgba & 0xff) / 255.0f
    );
    glVertex2f(0, 0);
    glVertex2f(0, h);
    glVertex2f(w, h);
    glVertex2f(w, 0);
    glEnd();

    glPopMatrix();
}

void gl_triangle_with_border(
    float x1,
    float y1,
    float x2,
    float y2,
    float x3,
    float y3,
    unsigned int fill_rgba = 0xFF000000,
    unsigned int border_rgba = 0x000000FF
    ) {
    glPushMatrix();

    glBegin(GL_TRIANGLES);
    glColor4f(
        static_cast<float>((fill_rgba >> 24) & 0xff) / 255.0f,
        static_cast<float>((fill_rgba >> 16) & 0xff) / 255.0f,
        static_cast<float>((fill_rgba >> 8) & 0xff) / 255.0f,
        static_cast<float>(fill_rgba & 0xff) / 255.0f
    );
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
    glEnd();

    // border
    glBegin(GL_LINE_LOOP);
    glColor4f(
        static_cast<float>((border_rgba >> 24) & 0xff) / 255.0f,
        static_cast<float>((border_rgba >> 16) & 0xff) / 255.0f,
        static_cast<float>((border_rgba >> 8) & 0xff) / 255.0f,
        static_cast<float>(border_rgba & 0xff) / 255.0f
    );
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
    glEnd();

    glPopMatrix();
}


int main() {
    const Window window("Lab1-2", 800, 600);
    window.loop([] {
        gl_rect_with_border(500, 400, 200, 200, 0xFFFF00ff);
        gl_rect_with_border(530, 430, 140, 140, 0xFF0000ff);
        gl_rect_with_border(550, 450, 100, 100, 0x007F00ff);

        gl_triangle_with_border(580, 510, 610, 510, 595, 530, 0x0000FFff);
        gl_rect_with_border(590, 500, 10, 10, 0x000000ff);

    });
    return 0;
}