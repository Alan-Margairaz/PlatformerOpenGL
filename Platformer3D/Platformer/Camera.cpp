#include "Camera.h"

// Constructeur:
Camera::Camera(float Speed, float RotationSpeed, double LastTime, float gravity, float velocity, float playerVelocityX, float playerVelocityY)
{
	cameraPos = glm::vec3(0, 1.8, 10);
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

	speed = Speed;
	rotationSpeed = RotationSpeed;
	lastTime = lastTime;

	gravity = gravity;
	velocity = velocity;

	playerVelocityX = playerVelocityX;
	playerVelocityY = playerVelocityY;
	playerXAxis += cameraPos.x;
	playerYAxis += cameraPos.y;
}

// Destructeur:
Camera::~Camera()
{

}

// View de la cam�ra:
glm::mat4 Camera::getView()
{
	return viewMatrix;
}

// Mouvement de la cam�ra:
void Camera::moveCamera(GLFWwindow* window)
{
	// R�cup�ration du temps pour le d�placement:
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	lastTime = currentTime;

	// Variables de gravit� et v�locit� pour le saut:
	gravity = 9.81f;
	velocity += gravity * deltaTime;
	cameraPos += gravity * deltaTime;

	// Aller vers l'avant
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		cameraPos += direction * deltaTime * speed;

	// Aller vers l'arri�re
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= direction * deltaTime * speed;

	// Pas � droite
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += right * deltaTime * speed;

	// Pas � gauche
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraPos -= right * deltaTime * speed;

	// Tourne � gauche
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		horizontalAngle += rotationSpeed * deltaTime;


	// Tourne � droite
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		horizontalAngle -= rotationSpeed * deltaTime;

	// Saut 
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		playerVelocityY -= gravity;

		playerXAxis += playerVelocityX;
		playerYAxis += playerVelocityY;
	}	

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
		cameraPos,				// cameraPos de la cam ra (4,3,3) dans l'espace monde
		cameraPos + direction,	// Direction de la cam ra (ce qu'elle regarde)
		up						// Vecteur Up (utilisez 0,-1,0 pour regarder � l'envers)
	);
}

glm::vec3 Camera::getPos()
{
	return cameraPos;
}


