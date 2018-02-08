#pragma once

class VertexBuffer {
private:
	unsigned int m_RendererId;
	bool m_freeMemory = true;

public:
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer();
	~VertexBuffer();

	void load(const void* data, unsigned int size);

	void Bind() const;
	void Unbind() const;

	void setDelete(bool del) {
		m_freeMemory = del;
	}
};
