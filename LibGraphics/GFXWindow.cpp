#include "GFXWindow.h"

void GFXWindow::setGFXWindowInstanceCallbacks(GFXWindow& windowInstance) {

}

//
//
//

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

bool GFXWindow::isKeyPressed(int key) {
	return (bool)glfwGetKey(this->gfxWindow, key);
}

void GFXWindow::setKeyCallback(GLFWkeyfun callback) {
	glfwSetKeyCallback(this->gfxWindow, callback);
}

void GFXWindow::setScrollWheelCallback(GLFWscrollfun callback) {
	glfwSetScrollCallback(this->gfxWindow, callback);
}


void GFXWindow::setInputMode(int mode, int value) {
	glfwSetInputMode(this->gfxWindow, mode, value);
}

bool GFXWindow::makeContextCurrent() {
	glfwMakeContextCurrent(this->gfxWindow);
	return glewInit() == GLEW_OK;
}

bool GFXWindow::shouldClose() {
	return glfwWindowShouldClose(this->gfxWindow);
}