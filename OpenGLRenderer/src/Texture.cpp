#include "Texture.h"
#include "Renderer.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION //for stb
#include <stb_image.h>

Texture::Texture()
	: m_layerCount(0)
	, m_Size({0, 0})
{
	stbi_set_flip_vertically_on_load(true);
	GLCall(glGenTextures(1, &m_RendererId));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererId));
	setFilteringMin(FilterType::NEAREST_MIPMAP);
	setFilteringMag(FilterType::LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture::Texture(const std::string& filepath)
	: m_layerCount(0)
	, m_Size({ 0, 0 }) {
	stbi_set_flip_vertically_on_load(true);
	GLCall(glGenTextures(1, &m_RendererId));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererId));
	setFilteringMin(FilterType::NEAREST_MIPMAP);
	setFilteringMag(FilterType::LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	loadFromFile(filepath);
}

Texture::~Texture() {
	GLCall(glDeleteTextures(1, &m_RendererId));
}

void Texture::logInfo() {
	std::cout << "Layers: " << getLayerCount() << ", Size: " << getSize().x << "x" << getSize().y << std::endl;
}

glm::vec2 Texture::getSize() {
	return m_Size;
}

unsigned int Texture::getLayerCount() {
	return m_layerCount;
}

void Texture::Bind(unsigned short unit) const { // maximum unit is 15 (range: 0 - 15)
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, m_RendererId);
}

void Texture::Unbind() const {
	for (unsigned short i = 0; i < 15; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Texture::setFilteringMin(FilterType t) {
	unsigned int param = 0;
	switch (t) {
		case FilterType::LINEAR:
			param = GL_LINEAR;
			break;
		case FilterType::LINEAR_MIPMAP:
			param = GL_LINEAR_MIPMAP_LINEAR;
			break;
		case FilterType::NEAREST:
			param = GL_NEAREST;
			break;
		case FilterType::NEAREST_MIPMAP:
			param = GL_NEAREST_MIPMAP_NEAREST;
	}
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param));
}

void Texture::setFilteringMag(FilterType t) {
	unsigned int param = 0;
	switch (t) {
		case FilterType::LINEAR:
			param = GL_LINEAR;
			break;
		case FilterType::LINEAR_MIPMAP:
			param = GL_LINEAR_MIPMAP_LINEAR;
			break;
		case FilterType::NEAREST:
			param = GL_NEAREST;
			break;
		case FilterType::NEAREST_MIPMAP:
			param = GL_NEAREST_MIPMAP_NEAREST;
	}
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param));
}

void Texture::loadFromData(unsigned char * data, int& width, int& height) {
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
}

void Texture::loadFromFile(const std::string& filepath) {
	Bind(0);

	int x, y, layers;
	unsigned char* data = stbi_load(filepath.c_str(), &x, &y, &layers, STBI_rgb);
	if (data) {
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
		m_Size = glm::vec2(x, y);
		m_layerCount = layers;
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	Unbind();
}


