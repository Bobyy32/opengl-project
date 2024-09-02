#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	// camera Attributes
	glm::vec3 Position;	// camera position
	glm::vec3 Front;	// direction camera is looking at
	glm::vec3 Up;		//upwards direction
	glm::vec3 Right;	//rightwards direction
	glm::vec3 WorldUp;  //world up direction

	// Euler Angles
	float Yaw;
	float Pitch;

	// camera options
	float speed;
	float sensitivity;

	// First mouse interaction
	bool firstMouse;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = -90.0f,
		float pitch = 0.0f,
		float speed = 0.01f,
		float sensitivity = 0.01f)
		: Position(position), WorldUp(up), Yaw(yaw), Pitch(pitch), speed(speed), sensitivity(sensitivity), firstMouse(true)
	{
		Front = glm::vec3(0.0f, 0.0f, -1.0f);
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
	~Camera() {}

	glm::mat4 GetViewMatrix();

	void ProcessInput(GLFWwindow* window);

	glm::vec3 GetPosition() { return Position; }

};

