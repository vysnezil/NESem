#include "Display.h"
#include "Logger.h"
#include <string>

Display::Display()
{
    glHelper::getInstance().glInit();
    window = glHelper::getInstance().window;

    texture = setupTexture();
    glHelper::getInstance().lasttime = glfwGetTime();
}

void Display::update(uint8_t* displayData1) {
    while (glfwGetTime() < glHelper::getInstance().lasttime + (1.0 / 60)) {
        //wait
    }
    glHelper::getInstance().lasttime += (1.0 / 60);

    Overlay::getInstance().update();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 240, 0, GL_RGB, GL_UNSIGNED_BYTE, displayData1);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDisable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 1, 1, 0, -1, 1);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(0, 0);
    glTexCoord2f(0, 1); glVertex2f(0, 1);
    glTexCoord2f(1, 1); glVertex2f(1, 1);
    glTexCoord2f(1, 0); glVertex2f(1, 0);
    glEnd();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    
    //setWindowFPS(window);
}

void Display::setWindowFPS(GLFWwindow* win)
{
    frames++;

    if (glfwGetTime() - lasttimeCounter >= 1.0) {
        char title[256];
        title[255] = '\0';

        snprintf(title, 255, "%s - [FPS: %3.2f]", "NESem", (float)frames);

        glfwSetWindowTitle(win, title);

        frames = 0;
        lasttimeCounter += 1.0;
    }
}

unsigned int Display::setupTexture() {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    return texture;
}