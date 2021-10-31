#include "GFXShader.h"

#pragma once

struct ConstCharPtrCmp {
	bool operator()(const char* a, const char* b) const;
};

class GFXShaderProgram
{
private:
	unsigned int id = 0;
	bool compiled = false, linked = false;
	std::vector<GFXShader> shaders;
	std::map<const char*, unsigned int, ConstCharPtrCmp> uniformLocations;

public:
	GFXShaderProgram();

	GFXShaderProgram& addShader(const GFXShader& shader);

	bool compile(FILE* errOut = stderr);
	bool link(FILE* errOut = stderr);

	unsigned int uniformLocation(const char* uniformName);

	void use() const;
};

