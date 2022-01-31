#pragma once
#include <cstdint>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class Display
{
public:
	Display();
	bool shouldClose();
	void update(uint8_t* displayData);
	void updateEvents();

private:
	void error_callback(int error, const char* description);
	GLuint createShader();
	unsigned int setupTexture();
	unsigned int setupBuffers();
};

