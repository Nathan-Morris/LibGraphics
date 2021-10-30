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
} GFXShaderVariableType;

struct GFXShaderUniformInfo {
	const char* varName;
	GFXShaderVariableType uniformType : 4;
	GFXShaderVariableType subUniformType : 4;
	unsigned int size;
};

class _GFXShader
{
private:
	unsigned short version = 330;
	std::vector<GFXShaderUniformInfo> uniforms;
	std::vector<char> code;
	GLuint layoutIdMax;
	GLuint shaderId;
	GLenum shaderType;

public:
	_GFXShader(GLenum shaderType);

	_GFXShader& addUniform(const char* name, GFXShaderVariableType type, GFXShaderVariableType subType, unsigned int size);
	_GFXShader& addUniform(const char* name, GFXShaderVariableType type, unsigned int size);
	_GFXShader& addUniform(const char* name, GFXShaderVariableType type);
	_GFXShader& addLayout();

	_GFXShader& setCode(const char* code);
	_GFXShader& setCode(std::istream& inStream);
};

