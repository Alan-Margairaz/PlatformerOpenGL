#include "Mesh.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../../External/STB/stb_image.h"


// Constructeur
Mesh::Mesh(glm::vec3 position, GLuint programID)
{
	// Identifiant pour notre variable uniforme "MVP"
	matrixID = glGetUniformLocation(programID, "MVP");

	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	model = glm::mat4(1.0f);
	model = glm::translate(model, position);
}

// Destructeur
Mesh::~Mesh()
{
	// Nettoie VBO, VAO, textures & évite les pertes de mémoire:
	glDeleteBuffers(1, &elementBuffer);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &colorBuffer);
	glDeleteBuffers(1, &uvBuffer);
	glDeleteBuffers(1, &matrixID);
	glDeleteVertexArrays(1, &vertexArrayID);
	glDeleteTextures(1, &texture);
}

// Création de la mesh
void Mesh::CreateMesh(GLfloat colorVBO[], const char* objectPath, const char* imagePath, int colorSize, GLuint programID, glm::mat4 projection, glm::mat4 view)
{
	MVP = projection * view * model;

	// Crée une texture OpenGL
	glGenTextures(1, &texture);

	// "Lie" la nouvelle texture créée : tous les appels suivants aux fonctions de texture vont modifier cette texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Répète si ne prend pas toute la face.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Lorsque l'on rétrécit l'image, utiliser un fondu linéaire entre deux MIP maps, chacune étant aussi filtrée linéairement
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// Lorsque l'on agrandit l'image (aucune MIP map plus grande n'est disponible), utiliser le filtrage LINEAIRE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	// Lit le fichier, appel glTexImage2D avec les bons paramètres
	int width, height, nrChannels;
	unsigned char* data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		// Générer les MIP maps. 
		glGenerateMipmap(GL_TEXTURE_2D);

		// Initialise "myTextureSampler"
		textureID = glGetUniformLocation(programID, "myTextureSampler");
	}
	else
	{
		printf("Failed to load texture");
	}
	stbi_image_free(data);

	// Lire le fichier .obj
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> vertices, normals;
	bool res = loadOBJ(objectPath, vertices, uvs, normals);

	// Stockage des composants de l'obj dans l'indexation:
	std::vector<glm::vec2> uvsIndex;
	std::vector<glm::vec3> verticesIndex, normalsIndex;
	indexVBO(vertices, uvs, normals, indices, verticesIndex, uvsIndex, normalsIndex);

	// Permet d'identifier notre tampon de sommets / Vertex Buffer Object (VBO)
	glGenBuffers(1, &vertexBuffer);																					// Génère un tampon et place l'identifiant dans 'vertexbuffer'
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);																	// Les commandes suivantes vont parler de notre tampon 'vertexbuffer'
	glBufferData(GL_ARRAY_BUFFER, verticesIndex.size() * sizeof(glm::vec3), &verticesIndex[0], GL_STATIC_DRAW);		// Fournit les sommets à OpenGL.

	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, colorSize, colorVBO, GL_STATIC_DRAW);

	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, uvsIndex.size() * sizeof(glm::vec2), &uvsIndex[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, normalsIndex.size() * sizeof(glm::vec3), &normalsIndex[0], GL_STATIC_DRAW);

	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
}

//void Mesh::CreateMeshNotTextured(GLuint programID, glm::mat4 Projection, glm::mat4 View, const char* objectPath, GLfloat* colorVBO, int colorSize)
//{
//	MVP = Projection * View * model; 
//
//	// Lire le fichier .obj
//	std::vector<glm::vec2> uvs;
//	std::vector<glm::vec3> vertices, normals;
//	bool res = loadOBJ(objectPath, vertices, uvs, normals);
//
//	// Stockage des composants de l'obj dans l'indexation:
//	std::vector<glm::vec2> uvsIndex;
//	std::vector<glm::vec3> verticesIndex, normalsIndex;
//	indexVBO(vertices, uvs, normals, indices, verticesIndex,
//			uvsIndex, normalsIndex);
//
//	// Permet d'identifier notre tampon de sommets / Vertex Buffer Object (VBO)
//	glGenBuffers(1, &vertexBuffer);																					// Génère un tampon et place l'identifiant dans 'vertexbuffer'
//	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);																	// Les commandes suivantes vont parler de notre tampon 'vertexbuffer'
//	glBufferData(GL_ARRAY_BUFFER, verticesIndex.size() * sizeof(glm::vec3), &verticesIndex[0], GL_STATIC_DRAW);		// Fournit les sommets à OpenGL.
//
//	glGenBuffers(1, &colorBuffer);
//	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
//	glBufferData(GL_ARRAY_BUFFER, colorSize, colorVBO, GL_STATIC_DRAW);
//
//	glGenBuffers(1, &uvBuffer);
//	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
//	glBufferData(GL_ARRAY_BUFFER, uvsIndex.size() * sizeof(glm::vec2), &uvsIndex[0], GL_STATIC_DRAW);
//
//	glGenBuffers(1, &normalBuffer);
//	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
//	glBufferData(GL_ARRAY_BUFFER, normalsIndex.size() * sizeof(glm::vec3), &normalsIndex[0], GL_STATIC_DRAW);
//
//	glGenBuffers(1, &elementBuffer);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
//}


void Mesh::UpdateMVP(glm::mat4 projection, glm::mat4 view)
{
	MVP = projection * view * model;
}

void Mesh::Draw(GLFWwindow* window, GLuint programID)
{
	// Envoie notre transformation au shader dans la variable uniforme "MVP"
	// Pour chaque modèle, la MVP sera différente (au moins la partie M)
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);

	// Lie texture à GL_TEXTURE0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Lie "myTextureSampler" à GL_TEXTURE0
	glUniform1i(textureID, 0);

	// Premier tampon d'attributs : les sommets
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(
		0,                  // attribut 0. Aucune raison particulière pour 0, mais cela doit correspondre au layout dans le shader 
		3,                  // taille
		GL_FLOAT,           // type 
		GL_FALSE,           // normalis 
		0,                  // nombre d'octets parant deux sommets dans le tampon
		(void*)0            // décalage du tableau de tampon
	);

	// deuxième tampon d'attributs : Couleurs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glVertexAttribPointer(
		1,
		3,                                // taille
		GL_FLOAT,                         // type
		GL_FALSE,
		0,                                // nombre d'octets séparant deux sommets dans le tampon
		(void*)0                          // décalage du tableau de tampons
	);

	// troisième tampon d'attributs : UVs
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glVertexAttribPointer(
		2,
		2,                                // taille : U+V => 2
		GL_FLOAT,                         // type
		GL_FALSE,
		0,                                // nombre d'octets séparant deux sommets dans le tampon
		(void*)0                          // décalage du tableau de tampons
	);

	// quatrième tampon d'attributs : normales
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(
		3,
		3,                                // taille : Normales => 3
		GL_FLOAT,                         // type
		GL_FALSE,
		0,                                // nombre d'octets séparant deux sommets dans le tampon
		(void*)0                          // décalage du tableau de tampons
	);

	// cinquième tampon d'attributs : les indices (dessine les éléments)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glDrawElements(
		GL_TRIANGLES,			// mode
		indices.size(),			// nombre
		GL_UNSIGNED_SHORT,		// type
		(void*)0				// décalage du tableau de tampons
	);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

std::vector<unsigned short> Mesh::getIndices()
{
	return indices;
}



