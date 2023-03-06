#pragma once

#include <GLFW/glfw3.h>

#include "../../../External/GLM/glm/glm.hpp"
#include "../../../External/GLM/glm/ext/matrix_transform.hpp"


class Camera
{
private:
	glm::vec3 cameraPos, direction, right, up, playerXAxis, playerYAxis;
	glm::mat4 viewMatrix;
	float speed, horizontalAngle, verticalAngle, rotationSpeed, gravity, velocity, playerVelocityX, playerVelocityY;
	double lastTime;


public:
	Camera(float Speed, float RotationSpeed, double LastTime, float gravity, float velocity, float playerVelocityX, float playerVelocityY);
	~Camera();

	glm::mat4 getView();
	void moveCamera(GLFWwindow* window);
	glm::vec3 getPos();
};

