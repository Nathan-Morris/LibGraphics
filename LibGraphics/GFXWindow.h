#include "GFXGlobalInclude.h"

#pragma once

class GFXWindow
{
public/*private*/:
	GLFWwindow* gfxWindow;

	GFXWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);

public:
	GFXWindow(int width, int height, const char* title);
	GFXWindow(int width, int height, const char* title, const GFXWindow& share);

	void swapBuffers();

	bool makeContextCurrent();
	bool shouldClose();
};

