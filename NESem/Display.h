#pragma once
#include <cstdint>
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
	GLuint createShader();
	unsigned int setupTexture();
	unsigned int setupBuffers();
	double lasttime;
	double lasttimeCounter;
	uint16_t frames;
};

