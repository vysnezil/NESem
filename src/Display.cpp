#include "Display.h"
#include "Logger.h"
#include <string>

Display::Display()
{
	if (!glfwInit()) Logger::log("Failed to initialize GLFW!");
	window = glfwCreateWindow(768, 720, "NESem", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) Logger::log("Failed to initialize GLAD!");
    glfwSetErrorCallback(this->error_callback);
    glfwSetFramebufferSizeCallback(window, this->size_callback);

    glfwSwapInterval(0);

    lasttime = glfwGetTime();
}

void Display::error_callback(int error, const char* description) {
    Logger::log("OpenGL Error: ", std::string(description));
}

void Display::size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

bool Display::shouldClose() {
	return glfwWindowShouldClose(window);
}

void Display::update(uint8_t* displayData1) {
    while (glfwGetTime() < lasttime + (1.0 / 60)) {
        //wait
    }
    lasttime += (1.0 / 60);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (size_t y = 0; y < 240; y++)
    {
        for (size_t i = 0; i < 3; i++)
        {
            for (size_t x = 0; x < 256; x++)
            {
                int index = (256*240-1)*3 - (y * 256 + x) * 3;
                int indey = ((y*3 + i) * 256 + (255 - x)) * 9;
                dat[indey] = displayData1[index];
                dat[indey + 1] = displayData1[index + 1];
                dat[indey + 2] = displayData1[index + 2];

                dat[indey + 3] = displayData1[index];
                dat[indey + 4] = displayData1[index + 1];
                dat[indey + 5] = displayData1[index + 2];

                dat[indey + 6] = displayData1[index];
                dat[indey + 7] = displayData1[index + 1];
                dat[indey + 8] = displayData1[index + 2];
            }
        }           
    }
    

    glDrawPixels(768, 720, GL_RGB, GL_UNSIGNED_BYTE, dat);


    glfwSwapBuffers(window);
    setWindowFPS(window);
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

void Display::updateEvents()
{
    glfwPollEvents();
}
