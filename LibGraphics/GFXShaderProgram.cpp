#include "GFXShaderProgram.h"

bool ConstCharPtrCmp::operator()(const char* a, const char* b) const {
	return strcmp(a, b) < 0;
}

//
//
//

GFXShaderProgram::GFXShaderProgram() {

}

GFXShaderProgram& GFXShaderProgram::addShader(const GFXShader& shader) {
	this->shaders.push_back(shader);
	return *this;
}

bool GFXShaderProgram::compile(FILE* errOut) {
	if (this->compiled) {
		return true;
	}

	std::vector<char> compilationLogBuffer;
	int compilationLogLength;
	unsigned int shaderId;
	const char* shaderSrcPtr;
	int shaderSrcLen;

	for (GFXShader& shader : this->shaders) {
		shaderId = glCreateShader(shader.type());
		shaderSrcPtr = shader.codePtr();
		shaderSrcLen = (int)shader.codeLen();

		glShaderSource(shaderId, 1, &shaderSrcPtr, &shaderSrcLen);
		glCompileShader(shaderId);

		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &compilationLogLength);

		if (compilationLogLength) {
			compilationLogBuffer.resize(compilationLogLength + 1);
			glGetShaderInfoLog(shaderId, compilationLogLength, NULL, compilationLogBuffer.data());
			fputs(compilationLogBuffer.data(), errOut);
			throw std::runtime_error("Failed To Compile GFXShader");
			return false;
		}

		shader.id() = shaderId;
	}

	return (this->compiled = true);
}

bool GFXShaderProgram::link(FILE* errOut ) {
	if (this->linked) {
		return true;
	}

	// todo check if already linked
	std::vector<char> linkingLogBuffer;
	int linkingLogLength;

	this->id = glCreateProgram();

	for (const GFXShader& shader : this->shaders) {
		glAttachShader(this->id, shader.id());
	}

	glLinkProgram(this->id);

	glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &linkingLogLength);
	
	if (linkingLogLength) {
		linkingLogBuffer.resize(linkingLogLength + 1);
		glGetShaderInfoLog(this->id, linkingLogLength, NULL, linkingLogBuffer.data());
		fputs(linkingLogBuffer.data(), errOut);
		throw std::runtime_error("Failed To Link GFXShaderProgram");
		return false;
	}

	for (const GFXShader& shader : this->shaders) {
		glDetachShader(this->id, shader.id());
		glDeleteShader(shader.id());
	}

	return (this->linked = true);
}

unsigned int GFXShaderProgram::uniformLocation(const char* uniformName) {
	auto uniformMapIter = this->uniformLocations.find(uniformName);
	if (uniformMapIter != this->uniformLocations.end()) {
		return uniformMapIter->second;
	}
	else {
		return this->uniformLocations.insert(
			std::pair<const char*, unsigned int>(uniformName, glGetUniformLocation(this->id, uniformName))
		).first->second;
	}
}

void GFXShaderProgram::use() const {
	if (this->id) {
		glUseProgram(this->id);
	}
}