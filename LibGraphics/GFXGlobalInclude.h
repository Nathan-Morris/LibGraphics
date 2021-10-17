#ifndef _GRAPHICS_GLOBAL_INC_
#define _GRAPHICS_GLOBAL_INC_

#pragma once

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#pragma comment(lib, "glew32")
#pragma comment(lib, "glfw3dll")

static inline bool __GFX_INIT__() {
	if (!glfwInit()) {
		exit(-10);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	return true;
}

const extern bool __GFX_INID__;

#endif