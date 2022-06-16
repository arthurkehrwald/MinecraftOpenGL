#include "Camera.h"

Camera::Camera(glm::vec3 pos, float yaw, float pitch) : pos(pos), yaw(yaw), pitch(pitch)
{
	calculateDirections();
	calculateView();
}

glm::mat4 Camera::getView()
{
	return view;
}

glm::vec3 Camera::getPos()
{
	return pos;
}

void Camera::setPos(glm::vec3 value)
{
	if (value == pos) { return; }
	pos = value;
	calculateView();
}

void Camera::processMoveInput(GLFWwindow* window, float deltaTime, bool planar)
{
	glm::vec3 forwardMoveDirection;
	if (planar)
	{
		forwardMoveDirection = glm::vec3(forward.x, 0.0f, forward.z);
		forwardMoveDirection = glm::normalize(forwardMoveDirection);
	}
	else
	{
		forwardMoveDirection = forward;
	}
	
	glm::vec3 direction = glm::vec3(0.0f);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		direction += forwardMoveDirection;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		direction -= forwardMoveDirection;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		direction -= right;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		direction += right;
	}

	if (direction != glm::vec3(0.0f))
	{
		pos += glm::normalize(direction) * moveSpeed * deltaTime;
		calculateView();
	}
}

void Camera::processLookInput(glm::vec2 offset)
{
	yaw += offset.x;
	pitch += offset.y;
	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	calculateDirections();

	calculateView();
}

void Camera::calculateView()
{
	view = glm::lookAt(pos, pos + forward, up);
}

void Camera::calculateDirections()
{
	forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward.y = sin(glm::radians(pitch));
	forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward = glm::normalize(forward);
	right = glm::normalize(glm::cross(forward, worldUp));
	up = glm::normalize(glm::cross(right, forward));
}