#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../../../External/GLM/glm/glm.hpp"

#include "../../../External/GLM/glm/ext/matrix_transform.hpp"
#include "../../../External/GLM/glm/ext/matrix_clip_space.hpp"

#include "objloader.h"
#include "VBOIndexer.h"
#include "Shader.h"


class Mesh
{
private:
	GLuint matrixID, textureID, modelID, vertexArrayID, texture, vertexBuffer, colorBuffer, uvBuffer, normalBuffer, elementBuffer;
	glm::mat4 model, MVP;
	std::vector<unsigned short> indices;
	float scale;

public:
	Mesh(glm::vec3 position, GLuint programID);
	~Mesh();

	void CreateMesh(GLfloat colorVBO[], const char* objectPath, const char* imagePath, int colorSize, GLuint ProgramID, glm::mat4 projection, glm::mat4 view);
	//void CreateMeshNotTextured(GLuint programID, glm::mat4 Projection, glm::mat4 View, const char* pathObj, GLfloat* colorBufferData, int sizeColorBuffer);

	void UpdateMVP(glm::mat4 projection, glm::mat4 view);
	void Draw(GLFWwindow* window, GLuint programID);

	std::vector<unsigned short> getIndices();
};