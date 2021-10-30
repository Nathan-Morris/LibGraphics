#include "_GFXShader.h"

_GFXShader::_GFXShader(GLenum shaderType) : shaderType(shaderType) { 

}

_GFXShader& _GFXShader::addUniform(const char* name, GFXShaderVariableType type, GFXShaderVariableType subType, unsigned int size) {
	GFXShaderUniformInfo uniformInfo = { 0 };
	uniformInfo.varName = name;
	uniformInfo.uniformType = type;

	if (type >= GFX_VEC) {
		uniformInfo.subUniformType = subType;
		uniformInfo.size = size;
	}

	this->uniforms.push_back(uniformInfo);

	return *this;
}

_GFXShader& _GFXShader::addUniform(const char* name, GFXShaderVariableType type, unsigned int size) {
	return this->addUniform(
		name,
		type,
		(type == GFX_VEC) ? GFX_FLOAT : GFX_NONE,
		size
	);
}

_GFXShader& _GFXShader::addUniform(const char* name, GFXShaderVariableType type) {
	return this->addUniform(
		name,
		type,
		GFX_NONE,
		0
	);
}


_GFXShader& _GFXShader::setCode(const char* code) {
	size_t len = strlen(code);
	this->code.resize(len);
	memcpy(this->code.data(), code, len);
	return *this;
}

_GFXShader& _GFXShader::setCode(std::istream& inStream) {
	inStream.seekg(0, std::ios::end);
	
	size_t len = inStream.tellg();

	if (len > 0) {
		this->code.resize(len);
		inStream.read(this->code.data(), len);
	}

	return *this;
}