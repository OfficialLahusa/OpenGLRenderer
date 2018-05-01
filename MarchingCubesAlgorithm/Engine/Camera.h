#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVTY = 0.1f;
const float ZOOM = 45.0f;

class Camera {
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float pMovementSpeed;
	float pMouseSensitivity;
	float pZoom;

	float fov;
	float aspectRatio;
	float near;
	float far;

	virtual glm::mat4 GetViewMatrix() = 0;
	virtual glm::mat4 GetProjectionMatrix() = 0;

	virtual void MouseRotate(float xoffset, float yoffset, GLboolean constrainPitch = true) = 0;
	virtual void Zoom(float yoffset) = 0;
};

class FirstPersonCamera : public Camera {

private:
	glm::mat4 m_ViewMatrix;

public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float fov;
	float aspectRatio;
	float near;
	float far;



	float Yaw;
	float Pitch;

	float pMovementSpeed;
	float pMouseSensitivity;
	float pZoom;


	FirstPersonCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
		: Front(glm::vec3(0.0f, 0.0f, -1.0f))
		, pMovementSpeed(SPEED)
		, pMouseSensitivity(SENSITIVTY)
		, pZoom(ZOOM) {
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	FirstPersonCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
		: Front(glm::vec3(0.0f, 0.0f, -1.0f))
		, pMovementSpeed(SPEED)
		, pMouseSensitivity(SENSITIVTY)
		, pZoom(ZOOM) {
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}


	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();

	void Translate(Camera_Movement direction, float deltaTime);
	void MouseRotate(float xoffset, float yoffset, GLboolean constrainPitch = true);
	void Zoom(float yoffset);

	//void lookAt(glm::vec3 pos);

private:
	void updateCameraVectors();
};

class ThirdPersonCamera : public Camera {

private:
	glm::mat4 m_ViewMatrix;

public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float fov;
	float aspectRatio;
	float near;
	float far;

	float minDist = 0.5f;
	float maxDist = 20.f;

	float Yaw;
	float Pitch;

	float pMovementSpeed;
	float pMouseSensitivity;
	float pZoom;


	ThirdPersonCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = YAW,
		float pitch = PITCH,
		float dist = 1.f)
		: Front(glm::vec3(0.0f, 0.0f, -1.0f))
		, pMovementSpeed(SPEED)
		, pMouseSensitivity(SENSITIVTY)
		, pZoom(dist) {
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	ThirdPersonCamera(float posX,
		float posY,
		float posZ,
		float upX,
		float upY,
		float upZ,
		float yaw,
		float pitch,
		float dist = 1.f)
		: Front(glm::vec3(0.0f, 0.0f, -1.0f))
		, pMovementSpeed(SPEED)
		, pMouseSensitivity(SENSITIVTY)
		, pZoom(dist) {
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}


	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();

	void setPosition(glm::vec3 center);
	void MouseRotate(float xoffset, float yoffset, GLboolean constrainPitch = true);
	void Zoom(float yoffset);

private:
	void updateCameraVectors();
};