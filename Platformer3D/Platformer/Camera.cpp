#include "Camera.h"
#include <iostream>

// Constructeur:
Camera::Camera(float Speed, float RotationSpeed, double LastTime)
{
	// Variables positionnement caméra:
	cameraPos = glm::vec3(0, 1.8, 20);
	horizontalAngle = 3.14f;
	verticalAngle = 0.0f;
	direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);
	up = glm::cross(right, direction);
	viewMatrix = glm::lookAt(cameraPos, cameraPos + direction, up);

	// Variables déplacements:
	speed = Speed;
	rotationSpeed = RotationSpeed;
	lastTime = LastTime;

	// Variables pour le saut:
	isJumping = false;
	isDescending = false;
	jumpHeight = 5.0f;
	jumpSpeed = 2.0f;
}

// Destructeur:
Camera::~Camera()
{

}

// View de la caméra:
glm::mat4 Camera::GetView()
{
	return viewMatrix;
}


// Mise à jour de la vue de la caméra:
void Camera::PlayerMovements(GLFWwindow* window)
{
	// Récupération du temps pour le déplacement:
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	lastTime = currentTime;

	speed = 20;

	// Déplacements joueur:
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		cameraPos += speed * direction * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= speed * direction * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraPos -= speed * right * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += speed * right * deltaTime;

	// Déplacement de la "tête"/caméra:
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		horizontalAngle += rotationSpeed * deltaTime;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		horizontalAngle -= rotationSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		verticalAngle += rotationSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		verticalAngle -= rotationSpeed * deltaTime;

	// Saut:
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		PlayerJump(deltaTime);
}

void Camera::PlayerJump(float deltaTime)
{
	float gravity = 9.81f;

	// Activation du jump si le joueur n'est pas déjà en train de sauter:
	if (!isJumping && cameraPos.y <= 1.8f)
		isJumping = true;

	// Si le joueur saute, on compare sa hauteur actuelle avec la hauteur de saut max:
	if (isJumping)
	{
		// Si la caméra a atteint la hauteur de saut max:
		if (cameraPos.y >= jumpHeight)
		{
			// Fin du saut:
			isJumping = false;
			isDescending = true;
		}
		else
		{
			// Saut:
			cameraPos.y += (jumpSpeed * deltaTime) + (0.05f * gravity);
		}
	}
	else if (isDescending && cameraPos.y >= 1.8f)
	{
		cameraPos.y -= (jumpSpeed * deltaTime) + (0.05f * gravity);
	}
	else
	{
		// Lorsque la caméra est au sol:
		cameraPos.y = 1.8f;
		isDescending = false;
	}
}


// Mouvement de la caméra:
void Camera::MoveCamera(GLFWwindow* window)
{
	direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	viewMatrix = glm::lookAt(
		cameraPos,				// cameraPos de la caméra (4,3,3) dans l'espace monde
		cameraPos + direction,	// Direction de la caméra (ce qu'elle regarde)
		up						// Vecteur Up (utilisez 0,-1,0 pour regarder à l'envers)
	);
}

glm::vec3 Camera::GetCameraPos()
{
	return cameraPos;
}

//// Test fonction de saut:
//void Camera::Jump()
//{
//	if (isJumping) return;
//
//	isJumping = true;
//	//jumpSpeed = jumpHeight;
//
//	initialPosition = cameraPos;
//}

//void Camera::UpdatedJump()
//{
//	double currentTime = glfwGetTime();
//	auto deltaTime = float(currentTime - jumpStartTime);
//
//	if(isJumping && deltaTime < jumpTime)
//	{
//		float deltaY = jumpSpeed * deltaTime + gravity;
//		jumpSpeed += gravity * deltaTime;
//
//		cameraPos = initialPosition + glm::vec3(0.0f, deltaY, 0.0f);
//	}
//	else if(isJumping && deltaTime > jumpTime)
//	{
//		isJumping = false;
//		float deltaYNeg = jumpSpeed * deltaTime + gravity;
//		jumpSpeed -= gravity * deltaTime;
//
//		cameraPos = initialPosition - glm::vec3(0.0f, deltaYNeg, 0.0f);
//	}
//}

