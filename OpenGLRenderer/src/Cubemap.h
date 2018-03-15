#pragma once

#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>

class Cubemap {

private:

	unsigned int m_RendererId;
	glm::vec2 m_Size;
	unsigned int m_layerCount;

public:

	Cubemap();
	//Texture order: right, left, top, bottom, back, front (+x,-x,+y,-y,+z,-z)
	Cubemap(const std::vector<std::string>& filepaths);
	~Cubemap();

	void logInfo();

	glm::vec2 getSize();
	unsigned int getLayerCount();

	void Bind(unsigned short unit) const;
	void Unbind() const;

	//Texture order: right, left, top, bottom, back, front (+x,-x,+y,-y,+z,-z)
	void loadFromFile(const std::vector<std::string>& filepaths);

};