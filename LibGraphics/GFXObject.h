#include "GFXGlobalInclude.h"

#pragma once

template<const GLuint dimensions>
struct GFXVertex {
	GLfloat vertex[dimensions] = { 0.f };

	GFXVertex() { }

	GFXVertex(GLfloat fill) {
		for (GLuint i = 0; i != dimensions; i++) {
			this->vertex[i] = fill;
		}
	}

	GFXVertex(const std::initializer_list<GLfloat>& initVertex) {
		std::initializer_list<GLfloat>::const_iterator initVertexIter = initVertex.begin();
		for (
			GLuint i = 0;
			(i != dimensions) && (initVertexIter != initVertex.end()); 
			i++, initVertexIter++
		) {
			this->vertex[i] = *initVertexIter;
		}
	}

	GLfloat& at(size_t offset) { return this->vertex[offset]; }
	
	GLfloat& operator[](size_t offset) { return this->vertex[offset]; }
};


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

	std::vector<GFXVertex<vertexDimensions>> vertices;

	GLuint VBO;

public:

	GFXObject(GFXObjectHints objHints) : hints(objHints) {
		glGenBuffers(1, &this->VBO);
	}

	~GFXObject() {
		glDeleteBuffers(1, &this->VBO);
	}
	
	void addVertex(const GFXVertex<vertexDimensions>& vertex) {
		this->vertices.push_back(vertex);
	}

	GFXVertex<vertexDimensions>& getVertex(size_t at) {
		return this->vertices.at(at);
	}

	void render(GLenum drawMode) {
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		if (!this->hints.staticVertices || !this->hints.bufferedVertices) {
			glBufferData(
				GL_ARRAY_BUFFER,
				sizeof(GFXVertex<vertexDimensions>) * vertices.size(),
				vertices.data(),
				GL_DYNAMIC_DRAW
			);

			this->hints.bufferedVertices = true;
		}
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
			this->vertices.size()
		);
		glDisableVertexAttribArray(0);
	}
};