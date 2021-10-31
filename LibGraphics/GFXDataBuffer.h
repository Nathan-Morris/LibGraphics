#include "GFXVertex.h"

#pragma once

template<const GLuint dataDimension>
class GFXDataBuffer {
private:
	GLuint id;
	GLenum type;
	std::vector<GFXVertex<dataDimension>> data;

public:
	GFXDataBuffer(GLenum type = GL_ARRAY_BUFFER) : type(type) {
		glGenBuffers(1, &this->id);
	}

	GFXDataBuffer(const GFXDataBuffer<dataDimension>& dataBufferRef) 
		: GFXDataBuffer(dataBufferRef.type) {
		this->data = dataBufferRef.data;
	}

	GFXDataBuffer(const std::initializer_list<GFXVertex<dataDimension>>& initVertices) : GFXDataBuffer((GLenum)GL_ARRAY_BUFFER) {
		this->data.insert(this->data.begin(), initVertices);
	}

	~GFXDataBuffer() {
		glDeleteBuffers(1, &this->id);
	}

	void bind() {
		glBindBuffer(this->type, this->id);
	}

	void dataTranslate(void(*translationCallback)(GFXVertex<dataDimension>&)) {
		//this->bind();
		for (GFXVertex<dataDimension>& dataVertex : this->data) {
			translationCallback(dataVertex);
		}
	}

	// returns number of elements in `data`
	size_t dataCount() const {
		return this->data.size();
	}

	// returns size in bytes of `data` array
	size_t dataLength() const {
		return this->dataCount() * sizeof(GFXVertex<dataDimension>);
	}

	GLfloat* dataPtr() {
		return (GLfloat*)this->data.data();
	}

	const GLfloat* dataPtr() const {
		return (GLfloat*)this->data.data();
	}

	// moves data in `data` to `id` buffer 
	void putData(bool staticDraw = false) {
		this->bind();
		glBufferData(
			this->type,
			this->dataLength(),
			this->data.data(),
			staticDraw ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW
		);
	}

	GFXVertex<dataDimension>& dataAt(size_t offset) {
		return this->data.at(offset);
	}

	void dataAdd(const GFXVertex<dataDimension>& data) {
		this->data.push_back(data);
	}

	unsigned int getId() {
		return this->id;
	}

public:
	GFXDataBuffer<dataDimension>& operator=(const GFXDataBuffer<dataDimension>& dataBufferRef) {
		this->type = dataBufferRef.type;
		this->data = dataBufferRef.data;
		return *this;
	}
};