#pragma once

#include "VertexBuffer.h"

class VertexBufferLayout; //Forward creation to eliminate the need for VertexBufferLayout.h and also Renderer.h inside it
//Is in .cpp of this file though

class VertexArray {
private:
	unsigned int m_RendererId;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;
};