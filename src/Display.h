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
	void update(uint8_t* displayData);
	void setWindowFPS(GLFWwindow* win);
	GLFWwindow* window;

private:
	unsigned int setupTexture();
	double lasttime = 0;
	double lasttimeCounter = 0;
	uint16_t frames = 0;
	unsigned int texture;
};

