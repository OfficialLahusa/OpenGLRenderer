#pragma once
#include <string>
#include <glm/glm.hpp>

enum class FilterType {
	NEAREST_MIPMAP, NEAREST, LINEAR_MIPMAP, LINEAR
};

class Texture {

private:

	unsigned int m_RendererId;
	glm::vec2 m_Size;
	unsigned int m_layerCount;

public:

	Texture();
	Texture(const std::string& filepath);
	~Texture();

	void logInfo();

	glm::vec2 getSize();
	unsigned int getLayerCount();

	void Bind(unsigned short unit) const;
	void Unbind() const;

	void setFilteringMin(FilterType t);
	void setFilteringMag(FilterType t);

	void loadFromData(unsigned char* data, int& width, int& height);
	void loadFromFile(const std::string& filepath);

};