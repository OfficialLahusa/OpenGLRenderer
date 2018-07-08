#include "Camera.h"
#include <iostream>
#include <glm/gtx/matrix_decompose.hpp>

DecomposedModelMatrix Camera::DecomposeModelMatrix(glm::mat4 matrix)
{
	DecomposedModelMatrix result;	// Output struct with all our important information, rotation is a vec3 instead of a Quaternion
	glm::quat untreatedRotation;	// Temporary storage for the Quaternion returned by glm::decompose()

	glm::decompose(matrix, result.scale, untreatedRotation, result.location, result.skew, result.perspective);

	result.rotation = glm::eulerAngles(untreatedRotation);	// Converting the temporary Quaternion into euler angles and storing them inside the result struct

	return result;
}

glm::vec3 Camera::EulerAnglesToDirectionVector(glm::vec3 angles)
{
	glm::vec3 direction;
	// Applying default formula for conversion
	direction.x = cos(glm::radians(angles.z)) * cos(glm::radians(angles.y));
	direction.y = sin(glm::radians(angles.y));
	direction.z = sin(glm::radians(angles.z)) * cos(glm::radians(angles.y));
	return direction;
}

glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix() {
	return glm::perspective(glm::radians(fov), aspectRatio, near, far);
}

void Camera::lookAt(glm::vec3 point)
{
	DecomposedModelMatrix transform = DecomposeModelMatrix(glm::lookAt(Position, point, Up));
	RollPitchYaw = transform.rotation;
}

/*void Camera::Translate(Camera_Movement direction, float deltaTime) {
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
}*/

void Camera::MouseRotate(float xoff, float yoff, GLboolean constrainPitch) {

	RollPitchYaw.z += xoff;
	RollPitchYaw.y += yoff;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch) {
		if (RollPitchYaw.y > 89.0f)
			RollPitchYaw.y = 89.0f;
		if (RollPitchYaw.y < -89.0f)
			RollPitchYaw.y = -89.0f;
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

void Camera::Translate(glm::vec3 amount) {
	Position += amount;
}

void Camera::updateCameraVectors() {

	// Set and normalize internal vectors for later usage
	Front = glm::normalize(EulerAnglesToDirectionVector(RollPitchYaw));
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}
