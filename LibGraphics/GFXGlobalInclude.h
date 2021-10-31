#ifndef _GRAPHICS_GLOBAL_INC_
#define _GRAPHICS_GLOBAL_INC_

#pragma once

#include <iostream>
#include <functional>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#pragma comment(lib, "glew32")
#pragma comment(lib, "glfw3dll")

#ifndef _GFX_GLFW_CONTEXT_VERSION_MAJOR
#	define	_GFX_GLFW_CONTEXT_VERSION_MAJOR 3
#endif

#ifndef _GFX_GLFW_CONTEXT_VERSION_MINOR
#	define	_GFX_GLFW_CONTEXT_VERSION_MINOR 3
#endif

static inline bool __GFX_INIT__() {
	glewExperimental = true;

	if (!glfwInit()) {
		exit(-10);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _GFX_GLFW_CONTEXT_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _GFX_GLFW_CONTEXT_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	return true;
}

const extern bool __GFX_INITD__;

#endif