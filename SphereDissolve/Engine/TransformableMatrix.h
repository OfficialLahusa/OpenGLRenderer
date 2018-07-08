#pragma once
#include <glm\glm.hpp>

class TransformableMatrix {
private:
	glm::mat4 m_Matrix;
	glm::vec3 m_totalPosition, m_totalScale, m_totalRotation;

public:

	TransformableMatrix(glm::vec3 pos = { 0.f, 0.f, 0.f }, glm::vec3 scale = { 1.f, 1.f, 1.f }, glm::vec3 rot = { 0.f, 0.f, 0.f });

	void translate(glm::vec3 v);
	void rotate(glm::vec3 v);
	void scale(glm::vec3 v);

	glm::mat4 getMatrix();
	glm::vec3 getPosition();
	glm::vec3 getRotation();
	glm::vec3 getScale();

	void setPosition(glm::vec3 v);
	void setRotation(glm::vec3 v);
	void setScale(glm::vec3 v);

	void reset();
};
