#include "GFXDataBuffer.h"
#include "GFXShaderProgram.h"

#pragma once

template<const GLuint vertexDimensions>
class GFXMaterial 
{
private:
	GLuint mVAO;
	GFXShaderProgram mShader;
	std::vector<GFXDataBuffer<vertexDimensions>> mVertexBuffers;

public:
	GFXMaterial(const GFXShaderProgram& shader) {
		glGenVertexArrays(1, &this->mVAO);
		this->bind();
		this->mShader = shader;
	}

	GFXMaterial(const GFXMaterial<vertexDimensions>& materialRef) {
		this->mVertexBuffers = materialRef.mVertexBuffers;
		this->bind();
	}

	~GFXMaterial() {
		glDeleteVertexArrays(1, &this->mVAO);
	}

	const GFXShaderProgram& shader() const {
		return this->mShader;
	}

	GFXShaderProgram& shader() {
		return this->mShader;
	}

	void putVertexBuffer(size_t layoutIndex, const GFXDataBuffer<vertexDimensions>& dataBuffer) {
		//this->bind();
		if (layoutIndex + 1 > this->mVertexBuffers.size()) {
			this->mVertexBuffers.resize(layoutIndex + 1);
		}
		this->mVertexBuffers.at(layoutIndex) = dataBuffer;
	}

	GFXDataBuffer<vertexDimensions>& getVertexBuffer(size_t layoutOffset) {
		return this->mVertexBuffers.at(layoutOffset);
	}

	const GFXDataBuffer<vertexDimensions>& getVertexBuffer(size_t layoutOffset) const {
		return this->mVertexBuffers.at(layoutOffset);
	}

	void enable() {
		for (size_t i = 0; i != this->mVertexBuffers.size(); i++) {
			GFXDataBuffer<vertexDimensions>& dataBuffer = this->mVertexBuffers.at(i);
			// TODO handle static data buffers
			dataBuffer.putData(false);
			glVertexAttribPointer(
				i, vertexDimensions, GL_FLOAT, GL_FALSE, sizeof(GFXVertex<vertexDimensions>), NULL
			);
			glEnableVertexAttribArray(i);
		}
	}

	void disable() const {
		for (size_t i = 0; i != this->mVertexBuffers.size(); i++) {
			glDisableVertexAttribArray(i);
		}
	}

	void bind() const {
		glBindVertexArray(this->mVAO);
	}

	void drawArrays(GLenum drawMode) {
		if (!this->mVertexBuffers.size())
			return;
		this->bind();
		this->enable();
		this->mShader.use();

		// TODO set uniform length for all vertex buffers in mVertexBuffers
		glDrawArrays(drawMode, 0, this->mVertexBuffers.at(0).dataCount());

		this->disable();
	}

public:
	GFXMaterial<vertexDimensions>& operator=(const GFXMaterial<vertexDimensions>& materialRef) {
		this->mVertexBuffers = materialRef.mVertexBuffers;
		return *this;
	}
};