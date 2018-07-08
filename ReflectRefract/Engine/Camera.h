#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW\glfw3.h>

#include <vector>

struct DecomposedModelMatrix {
	glm::vec3 location;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::vec3 skew;
	glm::vec4 perspective;
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVTY = 0.1f;
const float ZOOM = 45.0f;

class Camera {

private:
	void updateCameraVectors();

	DecomposedModelMatrix DecomposeModelMatrix(glm::mat4 matrix);
	glm::vec3 EulerAnglesToDirectionVector(glm::vec3 angles);

public:

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
		: Front(glm::vec3(0.0f, 0.0f, -1.0f))
		, pMovementSpeed(SPEED)
		, pMouseSensitivity(SENSITIVTY)
		, pZoom(ZOOM) {
		Position = position;
		WorldUp = up;
		RollPitchYaw.z = yaw;
		RollPitchYaw.y = pitch;
		updateCameraVectors();
	}
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
		: Front(glm::vec3(0.0f, 0.0f, -1.0f))
		, pMovementSpeed(SPEED)
		, pMouseSensitivity(SENSITIVTY)
		, pZoom(ZOOM) {
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		RollPitchYaw.z = yaw;
		RollPitchYaw.y = pitch;
		updateCameraVectors();
	}

	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	glm::vec3 RollPitchYaw;

	float pMovementSpeed;
	float pMouseSensitivity;
	float pZoom;

	float fov;
	float aspectRatio;
	float near;
	float far;

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();

	void lookAt(glm::vec3 point);

	void MouseRotate(float xoffset, float yoffset, GLboolean constrainPitch = true);
	void Zoom(float yoffset);
	void Translate(glm::vec3 amount);
};