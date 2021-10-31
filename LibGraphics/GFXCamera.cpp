#include "GFXCamera.h"

GFXCamera::GFXCamera(
	const glm::mat4& view,
	const glm::mat4& project,
	const glm::mat4& model
) : viewMatrix(view), projectMatrix(project), modelMatrix(model) {
	
}

GFXCamera::GFXCamera(
	const glm::vec3& cameraPos,
	const glm::vec3& cameraTarget,
	int width,
	int height,
	GLfloat fov
) {
	this->viewMatrix = glm::lookAt(
		cameraPos,
		cameraTarget,
		glm::vec3(0.f, 1.f, 0.f)
	);

	this->projectMatrix = glm::perspective(
		glm::radians(fov),
		(GLfloat)width / height,
		0.1f,
		100.f
	);

	this->modelMatrix = glm::mat4(1.f);
}

glm::mat4& GFXCamera::view() {
	return this->viewMatrix;
}

glm::mat4& GFXCamera::projection() {
	return this->projectMatrix;
}

glm::mat4& GFXCamera::model() {
	return this->modelMatrix;
}

void GFXCamera::loadModelViewProjection(GLuint location) {
	glm::mat4 modelViewProjection = this->projectMatrix * this->viewMatrix * this->modelMatrix;
	glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(modelViewProjection));
}