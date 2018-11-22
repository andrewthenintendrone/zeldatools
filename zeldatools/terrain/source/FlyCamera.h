#pragma once
#include "Camera.h"
#include <GLFW\glfw3.h>

enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class FlyCamera : public Camera
{
public:

	bool m_running = false;

	FlyCamera();

	void processKeyboard(Camera_Movement direction);
	void processMouseMovement(float xoffset, float yoffset);

private:

	void updateViewMatrix();

	glm::vec3 m_front = glm::vec3(0, 0, 1);
	glm::vec3 m_right = glm::vec3(1, 0, 0);
	glm::vec3 m_up = glm::vec3(0, 1, 0);

	float m_mouseSensitivity = 0.1f;

	float m_pitch = 0.0f;
	float m_yaw = -90.0f;

	float m_walkSpeed = 100.0f;
	float m_runSpeed = 500.0f;
};