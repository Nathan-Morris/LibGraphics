#include "GFXShader.h"

GFXShader::GFXShader(
	const char* vertexShaderSource,
	int vertexShaderSourceLen,
	const char* fragmentShaderSource,
	int fragmentShaderSourceLen
) : gfxVertexShaderSrc(vertexShaderSource), gfxVertexShaderSrcLen(vertexShaderSourceLen),
	gfxFragmentShaderSrc(fragmentShaderSource), gfxFragmentShaderSrcLen(fragmentShaderSourceLen)
{ }

GFXShader::GFXShader(
	const char* vertexShaderSource,
	const char* fragmentShaderSource
) : GFXShader(vertexShaderSource, strlen(vertexShaderSource), fragmentShaderSource, strlen(fragmentShaderSource)) { }

GLuint GFXShader::programId() {
	return this->gfxShaderProgramId;
}

GLuint GFXShader::vertexId() {
	return this->gfxVertexShaderId;
}

GLuint GFXShader::fragmentId() {
	return this->gfxFragmentShaderId;
}

GLuint GFXShader::getUniform(const char* uniformName) {
	return glGetUniformLocation(this->gfxShaderProgramId, uniformName);
}

void GFXShader::use() {
	glUseProgram(this->gfxShaderProgramId);
}

bool GFXShader::compile(FILE* errOutputFile) {

	GLint compilationLogLength;

	// vertex shader compilation
	this->gfxVertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(this->gfxVertexShaderId, 1, &this->gfxVertexShaderSrc, &this->gfxVertexShaderSrcLen);
	glCompileShader(this->gfxVertexShaderId);
	glGetShaderiv(this->gfxVertexShaderId, GL_INFO_LOG_LENGTH, &compilationLogLength);
	// vertex shader failed compilation
	if (compilationLogLength) {
		char* compilationLogBuffer = new char[compilationLogLength + 1]();
		glGetShaderInfoLog(this->gfxVertexShaderId, compilationLogLength, NULL, compilationLogBuffer);
		fputs(compilationLogBuffer, errOutputFile);
		delete[] compilationLogBuffer;
		return false;
	}

	// fragment shader compilation
	this->gfxFragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(this->gfxFragmentShaderId, 1, &this->gfxFragmentShaderSrc, &this->gfxFragmentShaderSrcLen);
	glCompileShader(this->gfxFragmentShaderId);
	glGetShaderiv(this->gfxFragmentShaderId, GL_INFO_LOG_LENGTH, &compilationLogLength);
	// fragment shader failed compilation
	if (compilationLogLength) {
		char* compilationLogBuffer = new char[compilationLogLength + 1]();
		glGetShaderInfoLog(this->gfxFragmentShaderId, compilationLogLength, NULL, compilationLogBuffer);
		fputs(compilationLogBuffer, errOutputFile);
		delete[] compilationLogBuffer;
		return false;
	}

	this->gfxShaderProgramId = glCreateProgram();

	glAttachShader(this->gfxShaderProgramId, this->gfxVertexShaderId);
	glAttachShader(this->gfxShaderProgramId, this->gfxFragmentShaderId);

	glLinkProgram(this->gfxShaderProgramId);

	glGetProgramiv(this->gfxShaderProgramId, GL_INFO_LOG_LENGTH, &compilationLogLength);

	if (compilationLogLength) {
		char* compilationLogBuffer = new char[compilationLogLength + 1]();
		glGetProgramInfoLog(this->gfxShaderProgramId, compilationLogLength, NULL, compilationLogBuffer);
		fputs(compilationLogBuffer, errOutputFile);
		delete[] compilationLogBuffer;
		return false;
	}

	// TODO detach & delete shaders

	return true;
}