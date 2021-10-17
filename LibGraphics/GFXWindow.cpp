#include "GFXWindow.h"

GFXWindow::GFXWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share) {
	this->gfxWindow = glfwCreateWindow(
		width, height, title, monitor, share
	);
}

GFXWindow::GFXWindow(int width, int height, const char* title)
:	GFXWindow(width, height, title, NULL, NULL) { }

GFXWindow::GFXWindow(int width, int height, const char* title, const GFXWindow& share) 
:	GFXWindow(width, height, title, NULL, share.gfxWindow) { }

void GFXWindow::swapBuffers() {
	glfwSwapBuffers(this->gfxWindow);
}

bool GFXWindow::makeContextCurrent() {
	glfwMakeContextCurrent(this->gfxWindow);
	return glewInit() == GLEW_OK;
}

bool GFXWindow::shouldClose() {
	return glfwWindowShouldClose(this->gfxWindow);
}