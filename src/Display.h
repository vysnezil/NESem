#pragma once
#include <cstdint>

#include "glHelper.h"
#include "Overlay.h"

#include <chrono>
#include <thread>

class Display
{
public:
	Display();
	ImFont* AddDefaultFont(float pixel_size);
	void update(uint8_t* displayData);
	void setWindowFPS(GLFWwindow* win);
	GLFWwindow* window;

private:
	GLuint createShader();
	unsigned int setupTexture();
	unsigned int setupBuffers();
	double lasttime = 0;
	double lasttimeCounter = 0;
	uint16_t frames = 0;
};

