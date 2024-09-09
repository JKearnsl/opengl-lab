
#include "window.h"

void gl_rect_with_border(float x, float y, float w, float h, unsigned int fill_rgba = 0xFF000000, unsigned int border_rgba = 0x000000FF) {
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

int main() {
    const Window window("Lab1-3", 800, 600);
    window.loop([] {
        gl_rect_with_border(500, 400, 200, 200, 0xBFBFBFff);
        gl_rect_with_border(530, 430, 140, 140, 0xFF00FFff);
        gl_rect_with_border(570, 470, 60, 60, 0x7FFFFFff);
        gl_rect_with_border(590, 490, 20, 20, 0xFFFFFFff, 0xffffffff);
        gl_rect_with_border(590, 510, 20, 2, 0x000000ff);
        gl_rect_with_border(590, 490, 20, 2, 0xBFBFBFff, 0xBFBFBFff);

    });
    return 0;
}