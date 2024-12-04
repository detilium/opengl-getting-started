#pragma once
#include <glad\glad.h>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default values;
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float MOVEMENT_SPEED = 2.5f;
const float MOUSE_SENSITIVITY = 0.1f;
const float FOV_LIMIT = 90.0f;

class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float Yaw;
	float Pitch;

	float MovementSpeed;
	float MouseSensitivity;
	float FOV;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(MOVEMENT_SPEED), MouseSensitivity(MOUSE_SENSITIVITY), FOV(FOV_LIMIT)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
	}

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(MOVEMENT_SPEED), MouseSensitivity(MOUSE_SENSITIVITY), FOV(FOV_LIMIT)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
	}

	glm::mat3 GetViewMAtrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if(direction == FORWARD)
			Position += Front * velocity;
		if(direction == BACKWARD)
			Position -= Front * velocity;
		if(direction == LEFT)
			Position -= Right * velocity;
		if(direction == RIGHT)
			Position += Right * velocity;
	}

	void ProcessMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		if (constrainPitch)
		{
			if(Pitch > 89.0f)
				Pitch = 89.0f;
			if(Pitch < -89.0f)
				Pitch = -89.0f;
		}
	}

	void ProcessMouseScroll(float yoffset)
	{
		FOV -= (float)yoffset;
		if(FOV < 1.0f)
			FOV = 1.0f;
		if(FOV > 90.0f)
			FOV = 90.0f;
	}

private:
	void updateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};