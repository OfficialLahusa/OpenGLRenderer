#pragma once
#include <glm/glm.hpp>

class PointLight {

private:
	glm::vec3 m_Pos = { 0.f, 0.f, 0.f }, m_Color = { 1.f, 1.f, 1.f };
	//float brightness = 1.f;

public:
	PointLight();
	PointLight(glm::vec3 pos, glm::vec3 color);
	~PointLight();

	void translate(glm::vec3 amount);

	glm::vec3 getPosition() {
		return m_Pos;
	}
	void setPosition(glm::vec3 pos) {
		m_Pos = pos;
	}
	glm::vec3 getColor() {
		return m_Color;
	}
	void setColor(glm::vec3 col) {
		m_Color = col;
	}

};