#include "GFXGlobalInclude.h"

#pragma once

class GFXShader
{
private:
	const char* gfxVertexShaderSrc;
	const char* gfxFragmentShaderSrc;

	int gfxVertexShaderSrcLen;
	int gfxFragmentShaderSrcLen;

	GLuint gfxShaderProgramId;
	GLuint gfxVertexShaderId;
	GLuint gfxFragmentShaderId;

public:
	GFXShader(
		const char* vertexShaderSource,
		int vertexShaderSourceLen,
		const char* fragmentShaderSource,
		int fragmentShaderSourceLen
	);

	GFXShader(
		const char* vertexShaderSource,
		const char* fragmentShaderSource
	);

	GLuint programId();
	GLuint vertexId();
	GLuint fragmentId();

	GLuint getUniform(const char* uniformName);

	void use();

	bool compile(FILE* errOutputFile = stderr);
};

