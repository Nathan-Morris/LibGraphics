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