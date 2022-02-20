#include "../include/textRenderer.hpp"
#include <GL/gl.h>

void TextRenderer::init() {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void TextRenderer::display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2i(20, 20);
    glBitmap(10, 12, 0.0, 0.0, 11.0, 0.0, rasters);
    glBitmap(10, 12, 0.0, 0.0, 11.0, 0.0, rasters);
    glBitmap(10, 12, 0.0, 0.0, 11.0, 0.0, rasters);
    glFlush();
}

void TextRenderer::reshape(int width, int height) {
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void TextRenderer::keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27:
            exit(0);
    }
}