#include "GFXShader.h"

#pragma once

class GFXCamera
{
private:
	glm::mat4 viewMatrix;
	glm::mat4 projectMatrix;
	glm::mat4 modelMatrix;

public:
	GFXCamera(
		const glm::mat4& view,
		const glm::mat4& project,
		const glm::mat4& model
	);

	GFXCamera(
		const glm::vec3& cameraPos,
		const glm::vec3& cameraTarget,
		int width,
		int height,
		GLfloat fov
	);

	glm::mat4& view();
	glm::mat4& projection();
	glm::mat4& model();

	void loadModelViewProjection(GLuint location);
};

