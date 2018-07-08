#include "Cubemap.h"
#include "Renderer.h"
#include <iostream>

/*#define STB_IMAGE_IMPLEMENTATION //for stb*/
#include <stb_image.h>

Cubemap::Cubemap()
	: m_layerCount(0)
	, m_Size({ 0, 0 })
{	

	GLCall(glGenTextures(1, &m_RendererId));
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererId));

	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
}

Cubemap::Cubemap(const std::vector<std::string>& filepaths)
	: m_layerCount(0)
	, m_Size({ 0, 0 })
{

	GLCall(glGenTextures(1, &m_RendererId));
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererId));

	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

	loadFromFile(filepaths);
}

Cubemap::~Cubemap() {
	GLCall(glDeleteTextures(1, &m_RendererId));
}

void Cubemap::logInfo() {
	std::cout << "Layers: " << getLayerCount() << ", Size: " << getSize().x << "x" << getSize().y << std::endl;
}

glm::vec2 Cubemap::getSize() {
	return m_Size;
}

unsigned int Cubemap::getLayerCount() {
	return m_layerCount;
}

void Cubemap::Bind(unsigned short unit) const {
	GLCall(glActiveTexture(GL_TEXTURE0 + unit));
	GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererId));
}

void Cubemap::Unbind() const {
	for (unsigned short i = 0; i < 15; i++) {
		GLCall(glActiveTexture(GL_TEXTURE0 + i));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
	}
}

void Cubemap::loadFromFile(const std::vector<std::string>& filepaths) {
	stbi_set_flip_vertically_on_load(false);
	int width, height, nrChannels;
	for (unsigned int i = 0; i < filepaths.size(); i++) {

		unsigned char *data = stbi_load(filepaths[i].c_str(), &width, &height, &nrChannels, 0);
		if (width != height) {
			std::cerr << "[Error] Width and height of cubemap texture " << filepaths[i] << " are not the same.\n";
		}
		if (data) {
			GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			));
			stbi_image_free(data);
		}
		else {
			std::cout << "[Error] Cubemap texture failed to load : " << filepaths[i] << std::endl;
			stbi_image_free(data);
		}
	}
	m_Size = { width, height };
	m_layerCount = nrChannels;
}
