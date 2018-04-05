#include "Light.h"

PointLight::PointLight() {

}

PointLight::PointLight(glm::vec3 pos, glm::vec3 color) 
	: m_Pos(pos)
	, m_Color(color)
{

}

PointLight::~PointLight() {

}

void PointLight::translate(glm::vec3 amount) {
	m_Pos += amount;
}
