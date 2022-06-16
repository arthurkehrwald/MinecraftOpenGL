#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
	Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), float yaw = 0.0f, float pitch = 0.0f);
	glm::mat4 getView();
	glm::vec3 getPos();
	void setPos(glm::vec3 value);
	void processMoveInput(GLFWwindow* window, float deltaTime, bool planar = false);
	void processLookInput(glm::vec2 offset);
private:
	glm::mat4 view;
	glm::vec3 pos;
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;
	const glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	float yaw;
	float pitch;
	const float moveSpeed = 10.0f;
	void calculateView();
	void calculateDirections();
};