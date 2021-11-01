#include "GFXGlobalInclude.h"

#pragma once

class GFXWindow
{
private:
	GLFWwindow* gfxWindow;

	GFXWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);

	void(*keyCallbackPtr)(unsigned int, unsigned int, ...);

private:
	static void setGFXWindowInstanceCallbacks(GFXWindow& windowInstance);

public:
	void keyboardCallback(GLFWwindow* windowPtr, unsigned int scanCode);

public:
	GFXWindow(int width, int height, const char* title);
	GFXWindow(int width, int height, const char* title, const GFXWindow& share);

	void swapBuffers();
	void setInputMode(int mode, int value);

	bool isKeyPressed(int key);

	//
	void setKeyCallback(GLFWkeyfun callback);
	void setScrollWheelCallback(GLFWscrollfun);

	bool makeContextCurrent();
	bool shouldClose();
};

