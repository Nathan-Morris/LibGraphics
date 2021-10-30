#include "GFXShader.h"

#pragma once

class GFXShaderProgram
{
private:
	GLuint id = 0;
	std::vector<GFXShader> shaders;

public:
	GFXShaderProgram();

	GFXShaderProgram& addShader(const GFXShader& shader);

	bool compile(FILE* errOut = stderr);
	bool link(FILE* errOut = stderr);

	inline unsigned int getUniform(const char* name) {
		return glGetUniformLocation(this->id, name);
	}

	void use();
};

