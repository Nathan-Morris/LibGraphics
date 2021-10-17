#include "GFXGlobalInclude.h"

#pragma once

class GFXShader
{
private:
	GLuint gfxShaderProgramId;

public:
	GFXShader(
		const char* vertexShaderSource,
		int vertexShaderSourceLen,
		const char* fragmentShaderSource,
		int fragmentShaderSourceLen,
		FILE* errorOutputFile = stderr
	);

	GFXShader(
		const char* vertexShaderSource,
		const char* fragmentShaderSource,
		FILE* errorOutputFile = stderr
	);

	GLuint programId();

	GLuint getUniform(const char* uniformName);

	void use();

	//bool compile(FILE* errOutputFile = stderr);
};

