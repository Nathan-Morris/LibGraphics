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