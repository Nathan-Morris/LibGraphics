#include "GFXWindow.h"
#include "GFXCamera.h"
#include "GFXMaterial.h"
#include "GFXShaderProgram.h"

#pragma once

using namespace std;
using namespace glm;

void GLAPIENTRY glMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	fprintf(stderr, "\nGL: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

typedef struct {
	float rotation, radius;
	vec3 pos;
} PointCameraInfo;

// awful solution
PointCameraInfo* CAMERA_INFO_PTR;

static void projectileMotion() {
	int WIDTH = 1024, HEIGHT = 512;

	GFXWindow window(WIDTH, HEIGHT, "ProjectileMotionApp");

	window.makeContextCurrent();
	window.setInputMode(GLFW_STICKY_KEYS, GL_TRUE);

	// enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(glMessageCallback, NULL);

	// enable z-buffer depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//
	// Graph 
	//
	GFXShaderProgram graphShaderProgram;
	graphShaderProgram.addShader(
		GFXShader(GL_VERTEX_SHADER).setCode(R"(
#version 330
layout(location = 0) in vec3 vertPos;
uniform mat4 graphMVP;
void main() {
	gl_Position = graphMVP * vec4(vertPos, 1);
}
		)")
	).addShader(
		GFXShader(GL_FRAGMENT_SHADER).setCode(R"(
#version 330
out vec3 fragColor;
void main() {
	fragColor = vec3(1, 1, 1);
}
		)")
	);
	GFXMaterial<3> graph(graphShaderProgram);
	graph.shader().compile();
	graph.shader().link	();
	graph.putVertexBuffer(0, {
		{ -1, 1, 0 },
		{ -1, -1, 0 },
		{ -1, -1, 0 },
		{ 1, -1, 0 }
	});

	//
	// X Vector
	//
	GFXShaderProgram xVectShaderProgram;
	xVectShaderProgram.addShader(
		GFXShader(GL_VERTEX_SHADER).setCode(R"(
#version 330
layout(location = 0) in vec3 vertPos;
uniform vec3 xVectOffset;
uniform mat4 xVectMVP;
void main() {
	gl_Position = xVectMVP * vec4((xVectOffset + vertPos), 1);
}
		)")
	).addShader(
		GFXShader(GL_FRAGMENT_SHADER).setCode(R"(
#version 330
out vec3 fragColor;
void main() {
	fragColor = vec3(1, 0, 1);
}
		)")
	);
	GFXMaterial<3> xVect(xVectShaderProgram);
	xVect.shader().compile();
	xVect.shader().link();
	xVect.putVertexBuffer(0, {
		{0, 0, 0},
		{1, 1, 0},
	});


	//
	// Camera View 
	//
	vec3 cameraLocation(0, 0, 3);
	vec3 cameraTarget(0, 0, 0);
	GFXCamera camera(
		cameraLocation,	// camera location
		cameraTarget,	// camera target
		WIDTH,
		HEIGHT,
		45.f			// fov
	);
	PointCameraInfo cameraInfo = {
		pi<float>() / 2.f, 4, vec3(0, 0, 0)
	};
	CAMERA_INFO_PTR = &cameraInfo;

	//
	// Input Handling
	//
	window.setKeyCallback(
		[](GLFWwindow* windowPtr, int key, int scanCode, int action, int mods) {
			PointCameraInfo* cameraInfoPtr = CAMERA_INFO_PTR;
			switch (key)
			{
			case GLFW_KEY_LEFT:
				cameraInfoPtr->rotation += .1f;
				break;
			case GLFW_KEY_RIGHT:
				cameraInfoPtr->rotation -= .1f;
				break;
			}
		}
	);
	window.setScrollWheelCallback(
		[](GLFWwindow* windowPtr, double xOffset, double yOffset) {
			PointCameraInfo* cameraInfoPtr = CAMERA_INFO_PTR;
			cameraInfoPtr->radius -= yOffset;
		}
	);

	//
	//
	//

	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// set camera position
		cameraInfo.pos[0] = cos(cameraInfo.rotation) * cameraInfo.radius;
		cameraInfo.pos[1] = 0;
		cameraInfo.pos[2] = sin(cameraInfo.rotation) * cameraInfo.radius;
		camera.view() = lookAt(cameraInfo.pos, vec3(0, 0, 0), vec3(0, 1, 0));

		// render graph
		graph.shader().use();
		camera.loadModelViewProjection(graph.shader().uniformLocation("graphMVP"));
		graph.drawArrays(GL_LINES);

		// render x vector
		xVect.shader().use();
		glUniform3f(xVect.shader().uniformLocation("xVectOffset"), -1, -1, 0);
		camera.loadModelViewProjection(xVect.shader().uniformLocation("xVectMVP"));
		xVect.drawArrays(GL_LINES);


		window.swapBuffers();
		glfwPollEvents();
	} while (!window.shouldClose());
}