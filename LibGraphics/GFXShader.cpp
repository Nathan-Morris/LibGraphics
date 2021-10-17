#include "GFXShader.h"

GFXShader::GFXShader(
	const char* vertexShaderSource,
	int vertexShaderSourceLen,
	const char* fragmentShaderSource,
	int fragmentShaderSourceLen,
	FILE* errOutputFile
) {
	GLint compilationLogLength;
	GLuint gfxVertexShaderId, gfxFragmentShaderId;

	// vertex shader compilation
	gfxVertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(gfxVertexShaderId, 1, &vertexShaderSource, &vertexShaderSourceLen);
	glCompileShader(gfxVertexShaderId);
	glGetShaderiv(gfxVertexShaderId, GL_INFO_LOG_LENGTH, &compilationLogLength);
	// vertex shader failed compilation
	if (compilationLogLength) {
		char* compilationLogBuffer = new char[compilationLogLength + 1]();
		glGetShaderInfoLog(gfxVertexShaderId, compilationLogLength, NULL, compilationLogBuffer);
		fputs(compilationLogBuffer, errOutputFile);
		delete[] compilationLogBuffer;
		throw std::runtime_error("Failed To Compile Vertex Shader Program");
	}

	// fragment shader compilation
	gfxFragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(gfxFragmentShaderId, 1, &fragmentShaderSource, &fragmentShaderSourceLen);
	glCompileShader(gfxFragmentShaderId);
	glGetShaderiv(gfxFragmentShaderId, GL_INFO_LOG_LENGTH, &compilationLogLength);
	// fragment shader failed compilation
	if (compilationLogLength) {
		char* compilationLogBuffer = new char[compilationLogLength + 1]();
		glGetShaderInfoLog(gfxFragmentShaderId, compilationLogLength, NULL, compilationLogBuffer);
		fputs(compilationLogBuffer, errOutputFile);
		delete[] compilationLogBuffer;
		throw std::runtime_error("Failed To Compile Fragment Shader Program");
	}

	this->gfxShaderProgramId = glCreateProgram();

	glAttachShader(this->gfxShaderProgramId, gfxVertexShaderId);
	glAttachShader(this->gfxShaderProgramId, gfxFragmentShaderId);

	glLinkProgram(this->gfxShaderProgramId);

	glGetProgramiv(this->gfxShaderProgramId, GL_INFO_LOG_LENGTH, &compilationLogLength);

	if (compilationLogLength) {
		char* compilationLogBuffer = new char[compilationLogLength + 1]();
		glGetProgramInfoLog(this->gfxShaderProgramId, compilationLogLength, NULL, compilationLogBuffer);
		fputs(compilationLogBuffer, errOutputFile);
		delete[] compilationLogBuffer;
		throw std::runtime_error("Failed To Link Shader Program");
	}

	// TODO detach & delete shaders

	glDetachShader(this->gfxShaderProgramId, gfxVertexShaderId);
	glDetachShader(this->gfxShaderProgramId, gfxFragmentShaderId);

	glDeleteShader(gfxVertexShaderId);
	glDeleteShader(gfxFragmentShaderId);
}

GFXShader::GFXShader(
	const char* vertexShaderSource,
	const char* fragmentShaderSource,
	FILE* errOutputFile
) : GFXShader(vertexShaderSource, strlen(vertexShaderSource), fragmentShaderSource, strlen(fragmentShaderSource), errOutputFile) { }

GLuint GFXShader::programId() {
	return this->gfxShaderProgramId;
}

GLuint GFXShader::getUniform(const char* uniformName) {
	return glGetUniformLocation(this->gfxShaderProgramId, uniformName);
}

void GFXShader::use() {
	glUseProgram(this->gfxShaderProgramId);
}

//bool GFXShader::compile(FILE* errOutputFile) {
//
//	GLint compilationLogLength;
//	GLuint gfxVertexShaderId, gfxFragmentShaderId;
//
//	// vertex shader compilation
//	gfxVertexShaderId = glCreateShader(GL_VERTEX_SHADER);
//	glShaderSource(gfxVertexShaderId, 1, &this->gfxVertexShaderSrc, &this->gfxVertexShaderSrcLen);
//	glCompileShader(gfxVertexShaderId);
//	glGetShaderiv(gfxVertexShaderId, GL_INFO_LOG_LENGTH, &compilationLogLength);
//	// vertex shader failed compilation
//	if (compilationLogLength) {
//		char* compilationLogBuffer = new char[compilationLogLength + 1]();
//		glGetShaderInfoLog(gfxVertexShaderId, compilationLogLength, NULL, compilationLogBuffer);
//		fputs(compilationLogBuffer, errOutputFile);
//		delete[] compilationLogBuffer;
//		return false;
//	}
//
//	// fragment shader compilation
//	gfxFragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(gfxFragmentShaderId, 1, &this->gfxFragmentShaderSrc, &this->gfxFragmentShaderSrcLen);
//	glCompileShader(gfxFragmentShaderId);
//	glGetShaderiv(gfxFragmentShaderId, GL_INFO_LOG_LENGTH, &compilationLogLength);
//	// fragment shader failed compilation
//	if (compilationLogLength) {
//		char* compilationLogBuffer = new char[compilationLogLength + 1]();
//		glGetShaderInfoLog(gfxFragmentShaderId, compilationLogLength, NULL, compilationLogBuffer);
//		fputs(compilationLogBuffer, errOutputFile);
//		delete[] compilationLogBuffer;
//		return false;
//	}
//
//	this->gfxShaderProgramId = glCreateProgram();
//
//	glAttachShader(this->gfxShaderProgramId, gfxVertexShaderId);
//	glAttachShader(this->gfxShaderProgramId, gfxFragmentShaderId);
//
//	glLinkProgram(this->gfxShaderProgramId);
//
//	glGetProgramiv(this->gfxShaderProgramId, GL_INFO_LOG_LENGTH, &compilationLogLength);
//
//	if (compilationLogLength) {
//		char* compilationLogBuffer = new char[compilationLogLength + 1]();
//		glGetProgramInfoLog(this->gfxShaderProgramId, compilationLogLength, NULL, compilationLogBuffer);
//		fputs(compilationLogBuffer, errOutputFile);
//		delete[] compilationLogBuffer;
//		return false;
//	}
//
//	// TODO detach & delete shaders
//
//	glDetachShader(this->gfxShaderProgramId, gfxVertexShaderId);
//	glDetachShader(this->gfxShaderProgramId, gfxFragmentShaderId);
//
//	glDeleteShader(gfxVertexShaderId);
//	glDeleteShader(gfxFragmentShaderId);
//
//	return true;
//}