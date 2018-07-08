#include "TransformableMatrix.h"
#include <glm/gtc/matrix_transform.hpp>

TransformableMatrix::TransformableMatrix(glm::vec3 pos, glm::vec3 scale, glm::vec3 rot) {
	m_Matrix = glm::rotate(glm::mat4(), rot.x, glm::vec3(1.f, 0.f, 0.f));
	m_Matrix = glm::rotate(m_Matrix, rot.y, glm::vec3(0.f, 1.f, 0.f));
	m_Matrix = glm::rotate(m_Matrix, rot.z, glm::vec3(0.f, 0.f, 1.f));

	m_Matrix = glm::scale(m_Matrix, scale);
	m_Matrix = glm::translate(m_Matrix, pos * (glm::vec3(1.f) / scale));

	m_totalPosition += pos * (glm::vec3(1.f) / scale);
	m_totalRotation = rot;
	m_totalScale = scale;
}

void TransformableMatrix::translate(glm::vec3 v) {
	m_Matrix = glm::translate(m_Matrix, v * (glm::vec3(1.f) / m_totalScale));

	m_totalPosition += v * (glm::vec3(1.f) / m_totalScale);
}

void TransformableMatrix::rotate(glm::vec3 v) {
	m_Matrix = glm::rotate(m_Matrix, v.x, glm::vec3(1.f, 0.f, 0.f));
	m_Matrix = glm::rotate(m_Matrix, v.y, glm::vec3(0.f, 1.f, 0.f));
	m_Matrix = glm::rotate(m_Matrix, v.z, glm::vec3(0.f, 0.f, 1.f));

	m_totalRotation += v;
}

void TransformableMatrix::scale(glm::vec3 v) {
	m_Matrix = glm::scale(m_Matrix, v);

	m_totalScale += v;
}

glm::mat4 TransformableMatrix::getMatrix() {
	return m_Matrix;
}

glm::vec3 TransformableMatrix::getPosition() {
	return m_totalPosition;
}

glm::vec3 TransformableMatrix::getRotation() {
	return m_totalRotation;
}

glm::vec3 TransformableMatrix::getScale() {
	return m_totalScale;
}

void TransformableMatrix::setPosition(glm::vec3 v) {
	m_Matrix = glm::rotate(glm::mat4(), m_totalRotation.x, glm::vec3(1.f, 0.f, 0.f));
	m_Matrix = glm::rotate(m_Matrix, m_totalRotation.y, glm::vec3(0.f, 1.f, 0.f));
	m_Matrix = glm::rotate(m_Matrix, m_totalRotation.z, glm::vec3(0.f, 0.f, 1.f));

	m_Matrix = glm::scale(m_Matrix, m_totalScale);

	m_Matrix = glm::translate(m_Matrix, v * (glm::vec3(1.f) / m_totalScale));

	m_totalPosition = v * (glm::vec3(1.f) / m_totalScale);
}

void TransformableMatrix::setRotation(glm::vec3 v) {
	m_Matrix = glm::rotate(glm::mat4(), v.x, glm::vec3(1.f, 0.f, 0.f));
	m_Matrix = glm::rotate(m_Matrix, v.y, glm::vec3(0.f, 1.f, 0.f));
	m_Matrix = glm::rotate(m_Matrix, v.z, glm::vec3(0.f, 0.f, 1.f));

	m_Matrix = glm::scale(m_Matrix, m_totalScale);
	m_Matrix = glm::translate(m_Matrix, m_totalPosition);

	m_totalRotation = v;
}

void TransformableMatrix::setScale(glm::vec3 v) {
	m_Matrix = glm::rotate(glm::mat4(), m_totalRotation.x, glm::vec3(1.f, 0.f, 0.f));
	m_Matrix = glm::rotate(m_Matrix, m_totalRotation.y, glm::vec3(0.f, 1.f, 0.f));
	m_Matrix = glm::rotate(m_Matrix, m_totalRotation.z, glm::vec3(0.f, 0.f, 1.f));

	m_Matrix = glm::scale(m_Matrix, v);
	m_Matrix = glm::translate(m_Matrix, m_totalPosition);

	m_totalScale = v;
}

void TransformableMatrix::reset() {
	glm::vec3 pos(0.f, 0.f, 0.f), scale(1.f, 1.f, 1.f), rot(0.f, 0.f, 0.f);
	m_Matrix = glm::rotate(glm::mat4(), rot.x, glm::vec3(1.f, 0.f, 0.f));
	m_Matrix = glm::rotate(m_Matrix, rot.y, glm::vec3(0.f, 1.f, 0.f));
	m_Matrix = glm::rotate(m_Matrix, rot.z, glm::vec3(0.f, 0.f, 1.f));

	m_Matrix = glm::scale(m_Matrix, scale);
	m_Matrix = glm::translate(m_Matrix, pos);

	m_totalPosition = pos;
	m_totalRotation = rot;
	m_totalScale = scale;
}
