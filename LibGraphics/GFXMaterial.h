#include "GFXDataBuffer.h"

#pragma once

template<const GLuint vertexDimensions>
class GFXMaterial 
{
private:
	GLuint VAO;

	std::vector<GFXDataBuffer<vertexDimensions>> vertexBuffers;

public:
	GFXMaterial() {
		glGenVertexArrays(1, &this->VAO);
	}

	~GFXMaterial() {
		glDeleteVertexArrays(1, &this->VAO);
	}

	void putVertexBuffer(size_t layoutIndex, const GFXDataBuffer<vertexDimensions>& dataBuffer) {
		if (layoutIndex > vertexBuffers.size()) {
			vertexBuffers.resize(layoutIndex + 1);
		}
		vertexBuffers.at(layoutIndex) = dataBuffer;
	}

	void enable() {
		for (size_t i = 0; i != this->vertexBuffers.size(); i++) {
			GFXDataBuffer<vertexDimensions>& dataBuffer = this->vertexBuffers.at(i);
			
			glEnableVertexAttribArray(i);

			glVertexAttribPointer(
				i, dataBuffer.dataLength(), GL_FLOAT, GL_FALSE, sizeof(GFXVertex<vertexDimensions>), dataBuffer.dataPtr()
			);
		}
	}



	void disable() {
		for (size_t i = 0; i != vertexBuffers; i++) {
			glDisableVertexAttribArray(i);
		}
	}
};