#include "Camera.h"

#include "Renderer.h"

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		Position += speed * Front;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		Position -= speed * Front;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		Position -= glm::normalize(glm::cross(Front, Up)) * speed;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		Position += glm::normalize(glm::cross(Front, Up)) * speed;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		Position += speed * Up;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		Position -= speed * Up;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		//hide cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		if (firstMouse)
		{
			glfwSetCursorPos(window, (double)WIDTH / 2, (double)HEIGHT / 2);
			firstMouse = false;
		}

		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		//change in position
		float xoffset = xPos - WIDTH / 2;
		float yoffset = HEIGHT / 2 - yPos;

		xoffset *= sensitivity;
		yoffset *= sensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		//constrain pitch
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;

		//update Front, Right and Up Vectors using the updated Euler angles
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		//update Right and Up Vectors
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
		
	}
	else
	{
		//unhide cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstMouse = true;
	}
}
