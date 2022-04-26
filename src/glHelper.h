#pragma once

#include <string>
#include "Logger.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class glHelper
{
public:
	void glInit();

	void render();

	bool shouldClose();

	void setupRender();
	void resizeWindow(bool menu);

	static void error_callback(int error, const char* description);
	static void size_callback(GLFWwindow* window, int width, int height);
	GLFWwindow* window;
	bool initialized;
	static glHelper& getInstance() {
		static glHelper instance;
		return instance;
	}

private:
	glHelper(){ };
};
