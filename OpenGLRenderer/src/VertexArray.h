#pragma once

#include "VertexBuffer.h"

#include <iostream>

class VertexBufferLayout; //Forward creation to eliminate the need for VertexBufferLayout.h and also Renderer.h inside it
//Is in .cpp of this file though

class VertexArray {
private:
	unsigned int m_RendererId;
	bool m_freeMemory = true;

public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;

	void setDelete(bool del) {
		m_freeMemory = del;
	}
};