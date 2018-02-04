#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray() {
	GLCall(glGenVertexArrays(1, &m_RendererId));
}

VertexArray::~VertexArray() {
	if (m_freeMemory)
		GLCall(glDeleteVertexArrays(1, &m_RendererId));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
	Bind();
	vb.Bind();
	unsigned int offset = 0;
	const std::vector<VertexBufferElement>& elements = layout.GetElements();
	for (unsigned int i = 0; i < elements.size(); i++) {
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, elements[i].count, elements[i].type, elements[i].normalized, layout.GetStride(), (const void*)offset));
		offset += elements[i].count * VertexBufferElement::GetSizeOfType(elements[i].type);
	}

}

void VertexArray::Bind() const {
	GLCall(glBindVertexArray(m_RendererId));
}

void VertexArray::Unbind() const {
	GLCall(glBindVertexArray(0));
}
