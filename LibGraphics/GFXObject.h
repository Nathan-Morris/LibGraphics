#include "GFXDataBuffer.h"

#pragma once


/*
Bit flags to specify what operations
need to be performed on vertices
prior to rendering
*/
typedef struct _gfx_object_hints{
	// flag to specify vertex information will not change, skips loading 
	// vertices into vertex buffer object each render call
	unsigned char staticVertices : 1;

	// flag to specify whether or not static vertex information has been
	// buffered into VBO
	unsigned char bufferedVertices : 1;

	// reserved
	unsigned char reserved : 6;
} GFXObjectHints, *pGFXObjectHints;
typedef const GFXObjectHints cGFXObjectHints, *pcGFXObjectHints;


template<const GLuint vertexDimensions>
class GFXObject {
protected:
	GFXObjectHints hints;
	GFXDataBuffer<vertexDimensions> data;

public:
	GFXObject(const GFXObjectHints& objHints) : hints(objHints) {}

	GFXObject() : GFXObject(GFXObjectHints({ 0 })) {}

	GFXObject(const std::initializer_list<GFXVertex<vertexDimensions>>& initVertices) : GFXObject(GFXObjectHints({ 0 })) {
		this->data = GFXDataBuffer<vertexDimensions>(initVertices);
	}

	~GFXObject() {}
	
	void addVertex(const GFXVertex<vertexDimensions>& vertex) {
		this->data.dataAdd(vertex);
	}
	
	GFXVertex<vertexDimensions>& getVertex(size_t at) {
		return this->data.dataAt(at);
	}

	void render(GLenum drawMode) {
		glEnableVertexAttribArray(0);
		
		this->data.putData(this->hints.staticVertices);

		//if (!this->hints.staticVertices || !this->hints.bufferedVertices) {
		//	this->data.putData(this->hints.staticVertices);
		//	this->hints.bufferedVertices = true;
		//}
		
		glVertexAttribPointer(
			0,
			vertexDimensions,
			GL_FLOAT,
			GL_FALSE,
			sizeof(GFXVertex<vertexDimensions>),
			NULL
		);
		
		glDrawArrays(
			drawMode,
			0,
			this->data.dataCount()
		);
		
		glDisableVertexAttribArray(0);
	}
};