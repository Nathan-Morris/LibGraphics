#include "GFXShader.h"

//
//
//

GFXShader::GFXShader(GLenum shaderType) : shaderType(shaderType) { 

}


GFXShader& GFXShader::setCode(const char* code) {
	size_t len = strlen(code);
	this->code.resize(len);
	memcpy(this->code.data(), code, len);
	return *this;
}

GFXShader& GFXShader::setCode(std::istream& inStream) {
	inStream.seekg(0, std::ios::end);
	
	size_t len = inStream.tellg();

	if (len > 0) {
		this->code.resize(len);
		inStream.read(this->code.data(), len);
	}

	return *this;
}

GFXShader& GFXShader::declareUniform(const char* name, GFXShaderDataType type, GFXShaderDataType subType, unsigned char size) {
	GFXShaderUniformInfo info = { 0 };
	info.name = name;
	info.type = type;

	switch (info.type)
	{
	case GFX_VEC:
	case GFX_MAT:
	case GFX_ARRAY:
		info.subType = subType;
		info.size = size;
		break;
	}

	this->uniforms.push_back(info);

	return *this;
}

GFXShader& GFXShader::declareUniform(const char* name, GFXShaderDataType type, unsigned char size) {
	return this->declareUniform(name, type, (type == GFX_VEC) ? GFX_FLOAT : GFX_NONE, size);
}


GFXShader& GFXShader::declareUniform(const char* name, GFXShaderDataType type) {
	if (type != GFX_NONE && type != GFX_VEC && type != GFX_MAT && type != GFX_ARRAY)
		return this->declareUniform(name, type, GFX_NONE, 0);
	return *this;
}

const char* GFXShader::codePtr() const {
	return this->code.data();
}

size_t GFXShader::codeLen() const {
	return this->code.size();
}

const GLuint& GFXShader::id() const {
	return this->shaderId; 
}

GLuint& GFXShader::id() {
	return this->shaderId;
}

GLenum GFXShader::type() const {
	return this->shaderType;
}