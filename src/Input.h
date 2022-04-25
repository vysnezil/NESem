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
	static bool saveFlag;
	static bool menuFlag;
	static bool loadFlag;
	static int keys[8];
	static int recordKey;
	static char* getArrow(int code);

	static void saveKeys();
	static void loadKeys();
};

