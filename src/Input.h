#pragma once
#include "Logger.h"
#include "glHelper.h"

class Input
{
public:
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	Input(GLFWwindow* window);
	static uint8_t controller;
	static bool singleStep;
	static bool step;
	static bool stepFrame;
};

