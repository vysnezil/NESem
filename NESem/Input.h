#pragma once
#include <GLFW/glfw3.h>
#include "Logger.h"

class Input
{
public:
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	Input(GLFWwindow* window);
	static uint8_t controller;
};

