#pragma once


class IndexBuffer {
private:
	unsigned int m_RendererId;
	unsigned int m_Count;
	bool m_freeMemory = true;

public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	IndexBuffer();
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }
	void setDelete(bool del) {
		m_freeMemory = del;
	}
};
