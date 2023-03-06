#pragma once

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
#include  "Shader.h"


class Mesh
{
private:
	GLuint matrixID, texture, textureID, vertexBuffer, colorBuffer, uvBuffer, normalBuffer, modelID, vertexArrayID, elementBuffer;
	glm::mat4 model;
	glm::mat4 MVP;
	std::vector<unsigned short> indices;

public:
	Mesh();
	~Mesh();

	void CreateMesh(GLfloat colorVBO[], const char* objectPath, const char* imagePath, int colorSize, GLuint ProgramID);
	void UpdateMVP(glm::mat4 projection, glm::mat4 view);
	void Draw(GLFWwindow* window, GLuint programID);
	std::vector<unsigned short> getIndices();
};