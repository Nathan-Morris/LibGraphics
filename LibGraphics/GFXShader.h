#include "GFXGlobalInclude.h"

#pragma once


// min = [0] ...0000 -> max = [8] ...1000
typedef enum : unsigned char {
	GFX_NONE = 0,
	GFX_BOOL = 1,
	GFX_INT = 2,
	GFX_UINT = 3,
	GFX_FLOAT = 4,
	GFX_DOUBLE = 5,

	GFX_VEC = 6,
	GFX_MAT = 7,
	GFX_ARRAY = 8,
} GFXShaderDataType;

class GFXShaderUniformInfo {
public:
	const char* name;
	unsigned char size;
	GFXShaderDataType type : 4;
	GFXShaderDataType subType : 4;
};

class GFXShader
{
private:
	unsigned short version = 330;
	std::vector<GFXShaderUniformInfo> uniforms;
	std::vector<char> code;
	GLuint layoutIdMax;
	GLuint shaderId = 0;
	GLenum shaderType;

public:
	GFXShader(GLenum shaderType);

	GFXShader& setCode(const char* code);
	GFXShader& setCode(std::istream& inStream);

	GFXShader& declareUniform(const char* name, GFXShaderDataType type, GFXShaderDataType subType, unsigned char size);
	GFXShader& declareUniform(const char* name, GFXShaderDataType type, unsigned char size);
	GFXShader& declareUniform(const char* name, GFXShaderDataType type);

	const char* codePtr() const;
	size_t codeLen() const;

	GLenum type() const;

	const GLuint& id() const;
	GLuint& id();
};

/*
#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 vertexColor;

uniform mat4 MVP;

out vec3 fragmentColor;

void main() {
	gl_Position = MVP * vec4(inPos, 1.0);
	fragmentColor = vertexColor;
}
*/
