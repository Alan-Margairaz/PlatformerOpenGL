#pragma once

#include <GLFW/glfw3.h>

#include "../../../External/GLM/glm/glm.hpp"
#include "../../../External/GLM/glm/ext/matrix_transform.hpp"


class Camera
{
private:
	glm::vec3 cameraPos, right, up, velocity;
	glm::mat4 viewMatrix;
	float speed, horizontalAngle, verticalAngle, rotationSpeed, jumpSpeed, jumpHeight;
	double lastTime;
	bool isJumping, isDescending;


public:
	Camera(float Speed, float RotationSpeed, double LastTime);
	~Camera();

	void PlayerMovements(GLFWwindow* window);
	void PlayerJump(float deltaTime);
	void MoveCamera(GLFWwindow* window);
	glm::mat4 GetView();
	glm::vec3 GetCameraPos();
	glm::vec3 direction;

	/*
	Tests de meilleurs jumps, sans succès:
	void UpdatedJump();	
	void Jump();
	*/
};

