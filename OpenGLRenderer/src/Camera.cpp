#include "Camera.h"
#include <iostream>

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix() {
	return glm::perspective(glm::radians(fov), aspectRatio, near, far);
}

void Camera::Translate(Camera_Movement direction, float deltaTime) {
	float velocity = pMovementSpeed * deltaTime;
	if (direction == FORWARD)
		Position += Front * velocity;
	if (direction == BACKWARD)
		Position -= Front * velocity;
	if (direction == LEFT)
		Position -= Right * velocity;
	if (direction == RIGHT)
		Position += Right * velocity;
	if (direction == DOWN)
		Position -= Up * velocity;
	if (direction == UP)
		Position += Up * velocity;
}

void Camera::MouseRotate(float xoff, float yoff, GLboolean constrainPitch) {

	Yaw += xoff;
	Pitch += yoff;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch) {
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}
	updateCameraVectors();
}

void Camera::Zoom(float yoffset) {
	if (pZoom >= 1.0f && pZoom <= 45.0f)
		pZoom -= yoffset;
	if (pZoom <= 1.0f)
		pZoom = 1.0f;
	if (pZoom >= 45.0f)
		pZoom = 45.0f;
}

/*void Camera::lookAt(glm::vec3 pos) {
	glm::vec3 dir = glm::normalize(pos);
	if (dir.z == 0)
		dir.z = 0.0000000001f;
	//Pitch = 1 * glm::atan(dir.x / dir.y);
	//Yaw = 1 * glm::atan(glm::sqrt(((dir.x * dir.x) + (dir.y * dir.y)) / dir.z));
	Pitch = glm::asin(-dir.y);
	Yaw = asin(dir.x / ((cos(Pitch)*glm::length(dir) == 0) ? 0.0000000001f : cos(Pitch)*glm::length(dir)));
	std::cout << Pitch << "/ " << Yaw << std::endl;
	updateCameraVectors();
}*/

void Camera::updateCameraVectors() {
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}