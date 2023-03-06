#include "Mesh.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../../External/STB/stb_image.h"


// Constructeur
Mesh::Mesh()
{

}

// Destructeur
Mesh::~Mesh()
{
	// Nettoie VBO, VAO, texture & �vite les pertes de m�moire
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &colorBuffer);
	glDeleteBuffers(1, &uvBuffer);
	glDeleteBuffers(1, &matrixID);
	glDeleteTextures(1, &texture);
	glDeleteVertexArrays(1, &vertexArrayID);
	glDeleteBuffers(1, &elementBuffer);
}

// Cr�ation de la mesh
void Mesh::CreateMesh(GLfloat colorVBO[], const char* objectPath, const char* imagePath, int colorSize, GLuint programID)
{
	// Identifiant pour notre variable uniforme "MVP"
	// Seulement au moment de l'initialisation.
	matrixID = glGetUniformLocation(programID, "MVP");

	// Cr e une texture OpenGL
	glGenTextures(1, &texture);

	// "Lie" la nouvelle texture cr  e : tous les appels suivants aux fonctions de texture vont modifier cette texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// R�p�te si ne prend pas toute la face.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Lorsque l'on r tr cit l'image, utiliser un fondu lin aire entre deux MIP maps, chacune  tant aussi filtr e lin airement
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// Lorsque l'on agrandit l'image (aucune MIP map plus grande n'est disponible), utiliser le filtrage LIN AIRE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	// Lit le fichier, appel glTexImage2D avec les bons param tres
	int width, height, nrChannels;
	unsigned char* data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		// G�n�rer les MIP maps. 
		glGenerateMipmap(GL_TEXTURE_2D);

		// Initialise "myTextureSampler"
		textureID = glGetUniformLocation(programID, "myTextureSampler");
	}
	else
	{
		printf("Failed to load texture");
	}
	stbi_image_free(data);

	GLuint vertexArrayID;	// Vertex Array Object (VAO) / 1 par objet / stocke les VBO
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	// Lire le fichier .obj
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> vertices, normals;
	bool res = loadOBJ(objectPath, vertices, uvs, normals);

	std::vector<glm::vec2> uvsIndex;
	std::vector<glm::vec3> verticesIndex, normalsIndex;
	indexVBO(vertices, uvs, normals, indices, verticesIndex, uvsIndex, normalsIndex);
	// Permet d'identifier notre tampon de sommets / Vertex Buffer Object (VBO)
	glGenBuffers(1, &vertexBuffer);																		// G�n�re un tampon et place l'identifiant dans 'vertexbuffer'
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);														// Les commandes suivantes vont parler de notre tampon 'vertexbuffer'
	glBufferData(GL_ARRAY_BUFFER, verticesIndex.size() * sizeof(glm::vec3), &verticesIndex[0], GL_STATIC_DRAW);	// Fournit les sommets � OpenGL.

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

	model = glm::mat4(1.0f);	// Changer pour chaque mod�le
}

void Mesh::UpdateMVP(glm::mat4 projection, glm::mat4 view)
{
	MVP = projection * view * model;
}

void Mesh::Draw(GLFWwindow* window, GLuint programID)
{
	// Envoie notre transformation au shader dans la variable uniforme "MVP"
	// Pour chaque mod le, la MVP sera diff�rente (au moins la partie M)
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);

	// Lie texture   GL_TEXTURE0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Lie "myTextureSampler"   GL_TEXTURE0
	glUniform1i(textureID, 0);

	// Premier tampon d'attributs : les sommets
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(
		0,                  // attribut 0. Aucune raison particuli�re pour 0, mais cela doit correspondre au layout dans le shader 
		3,                  // taille
		GL_FLOAT,           // type 
		GL_FALSE,           // normalis 
		0,                  // nombre d'octets parant deux sommets dans le tampon
		(void*)0            // d�calage du tableau de tampon
	);

	// deuxi�me tampon d'attributs : Couleurs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glVertexAttribPointer(
		1,
		3,                                // taille
		GL_FLOAT,                         // type
		GL_FALSE,
		0,                                // nombre d'octets s�parant deux sommets dans le tampon
		(void*)0                          // d�calage du tableau de tampons
	);

	// troisi�me tampon d'attributs : UVs
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glVertexAttribPointer(
		2,
		2,                                // taille : U+V => 2
		GL_FLOAT,                         // type
		GL_FALSE,
		0,                                // nombre d'octets s�parant deux sommets dans le tampon
		(void*)0                          // d�calage du tableau de tampons
	);

	// quatri�me tampon d'attributs : normales
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(
		3,
		3,                                // taille : Normales => 3
		GL_FLOAT,                         // type
		GL_FALSE,
		0,                                // nombre d'octets s�parant deux sommets dans le tampon
		(void*)0                          // d�calage du tableau de tampons
	);

	// cinqui�me tampon d'attributs : les indices (dessine les �l�ments)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glDrawElements(
		GL_TRIANGLES,			// mode
		indices.size(),			// nombre
		GL_UNSIGNED_SHORT,		// type
		(void*)0				// d�calage du tableau de tampons
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



