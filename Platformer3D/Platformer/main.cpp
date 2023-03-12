#include <GL/glew.h>

#include "Mesh.h"
#include "Camera.h"
#include "Shader.h"
#include "VBOIndexer.h"

GLFWwindow* window;
std::vector<Mesh> meshes;
glm::mat4 projection, view;

int main()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // Antialiasing 4x (4 fragment dans un pixel)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Version OpenGL 3.3 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Pas de support de l'ancien OpenGL

	// Ouvre une fenêtre et crée son contexte OpenGL
	window = glfwCreateWindow(1024, 768, "C++ Jeux 3D - OpenGL", NULL, NULL);
	if (window == NULL)
	{
		fprintf(stderr,"Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialise GLEW
	glewExperimental = true; // Nécessaire pour le profil core
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return -1;
	}

	// Permet d'utiliser la touche Echap:
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);


	// Fond d'écran (couleur bleu ciel):
	glClearColor(.4667f, .7098f, .9961f, 0.0f);


	// Active le test de profondeur
	glEnable(GL_DEPTH_TEST);

	// Accepte le fragment s'il est plus proche de la caméra que le précédent fragment accepté 
	glDepthFunc(GL_LESS);


	// Crée et compile notre programme GLSL à partir des shaders
	GLuint programID = LoadShaders("VertexShader.vert", "FragmentShader.frag");


	// Une couleur pour chaque sommet. Elles ont été générées aléatoirement. 
	GLfloat g_color_buffer_data_cube[] = {
	0.583f,  0.771f,  0.014f,
	0.609f,  0.115f,  0.436f,
	0.327f,  0.483f,  0.844f,
	0.822f,  0.569f,  0.201f,
	0.435f,  0.602f,  0.223f,
	0.310f,  0.747f,  0.185f,
	0.597f,  0.770f,  0.761f,
	0.559f,  0.436f,  0.730f,
	0.359f,  0.583f,  0.152f,
	0.483f,  0.596f,  0.789f,
	0.559f,  0.861f,  0.639f,
	0.195f,  0.548f,  0.859f,
	0.014f,  0.184f,  0.576f,
	0.771f,  0.328f,  0.970f,
	0.406f,  0.615f,  0.116f,
	0.676f,  0.977f,  0.133f,
	0.971f,  0.572f,  0.833f,
	0.140f,  0.616f,  0.489f,
	0.997f,  0.513f,  0.064f,
	0.945f,  0.719f,  0.592f,
	0.543f,  0.021f,  0.978f,
	0.279f,  0.317f,  0.505f,
	0.167f,  0.620f,  0.077f,
	0.347f,  0.857f,  0.137f,
	0.055f,  0.953f,  0.042f,
	0.714f,  0.505f,  0.345f,
	0.783f,  0.290f,  0.734f,
	0.722f,  0.645f,  0.174f,
	0.302f,  0.455f,  0.848f,
	0.225f,  0.587f,  0.040f,
	0.517f,  0.713f,  0.338f,
	0.053f,  0.959f,  0.120f,
	0.393f,  0.621f,  0.362f,
	0.673f,  0.211f,  0.457f,
	0.820f,  0.883f,  0.371f,
	0.982f,  0.099f,  0.879f
	};


	// Initialisation de lightColor et lightPos:
	GLuint lightColorID = glGetUniformLocation(programID, "lightColor");
	glm::vec3 lightColor = glm::vec3(1.0f);
	GLuint lightPosID = glGetUniformLocation(programID, "lightPos");
	glm::vec3 lightPos = glm::vec3(10.0f, 10.0f, 10.0f);

	// Initialisation de la caméra:
	Camera camera = Camera(3.0f, 1.0f, glfwGetTime());
	glm::vec3 viewPos = camera.GetCameraPos();
	view = camera.GetView();

	// Matrice de projection : Champ de vision de 45 °C, ratio 4:3, distance d'affichage : 0.1 unités <-> 100 unités 
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	// Initialisation des meshs contenus dans la scène:
	Mesh mesh1 = Mesh(glm::vec3(0,0,0), programID);
	mesh1.CreateMesh(g_color_buffer_data_cube, "Objects/cube.obj", "Objects/container.jpg", sizeof(g_color_buffer_data_cube), programID, projection, view);
	Mesh mesh2 = Mesh(glm::vec3(-1,0,-1), programID);
	mesh1.CreateMesh(g_color_buffer_data_cube, "Objects/cube.obj", "Objects/container.jpg", sizeof(g_color_buffer_data_cube), programID, projection, view);
	Mesh mesh3 = Mesh(glm::vec3(2, 0, 2), programID);
	mesh1.CreateMesh(g_color_buffer_data_cube, "Objects/cube.obj", "Objects/container.jpg", sizeof(g_color_buffer_data_cube), programID, projection, view);

	// Initialisation de viewPos:
	GLuint viewPosID = glGetUniformLocation(programID, "viewPos");

	// Appel de la fonction qui update la matrice MVP:
	mesh1.UpdateMVP(glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f), camera.GetView());
	mesh2.UpdateMVP(glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f), camera.GetView());
	mesh3.UpdateMVP(glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f), camera.GetView());

	// Initialisation du vector contenant plusieurs Mesh:
	meshes.push_back(mesh1);
	meshes.push_back(mesh2);
	meshes.push_back(mesh3);

	do
	{
		// Nettoie l'écran
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Utilise notre shader
		glUseProgram(programID);

		// Initialisation de mouvements de personnage/caméra:
		viewPos = camera.GetCameraPos();	// Update position caméra
		camera.MoveCamera(window);			// Update mouvement caméra
		camera.PlayerMovements(window);		// Mouvement personnage, comprend le saut

		glUniform3fv(lightPosID, 1, &lightPos[0]);
		glUniform3fv(viewPosID, 1, &viewPos[0]);
		glUniform3fv(lightColorID, 1, &lightColor[0]);

		view = camera.GetView();

		for (int i = 0; i < meshes.size(); i++)
		{
			meshes[i].UpdateMVP(glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f),camera.GetView());
			meshes[i].Draw(window, programID);
		}

		//  Echange les tampons
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Vérifie si la touche échap a été appuyée ou si la fenêtre a été fermée
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	glDeleteProgram(programID);

	glDisable(GL_CULL_FACE);

	// Ferme OpenGL et GLFW
	glfwTerminate();

	return 0;
}