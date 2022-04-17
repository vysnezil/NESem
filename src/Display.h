#pragma once
#include <cstdint>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <thread>

class Display
{
public:
	Display();
	bool shouldClose();
	void update(uint8_t* displayData);
	void setWindowFPS(GLFWwindow* win);
	void updateEvents();
	GLFWwindow* window;

private:
	static void error_callback(int error, const char* description);
	static void size_callback(GLFWwindow* window, int width, int height);
	GLuint createShader();
	unsigned int setupTexture();
	unsigned int setupBuffers();
	double lasttime = 0;
	double lasttimeCounter = 0;
	uint16_t frames = 0;
};

