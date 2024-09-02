#include "stb_image/stb_image.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>

#include "Texture.h"
#include "Renderer.h"
#include "Camera.h"

int main() {
	// Initialize GLFW
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	// Create a GLFW window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "opengl-project", nullptr, nullptr);
	if (!window) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	glViewport(0, 0, WIDTH, HEIGHT);
	stbi_set_flip_vertically_on_load(true);
	glEnable(GL_DEPTH_TEST);

	// Vertices coordinates
	float vertices[] =
	{ //     COORDINATES     /        COLORS        /    TexCoord    /       NORMALS     //
		-1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
		 1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
		 1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f
	};

	// Indices for vertices order
	unsigned int indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	float lightVertices[] =
	{ //     COORDINATES     //
		-0.1f, -0.1f,  0.1f,
		-0.1f, -0.1f, -0.1f,
		 0.1f, -0.1f, -0.1f,
		 0.1f, -0.1f,  0.1f,
		-0.1f,  0.1f,  0.1f,
		-0.1f,  0.1f, -0.1f,
		 0.1f,  0.1f, -0.1f,
		 0.1f,  0.1f,  0.1f
	};

	unsigned int lightIndices[] =
	{
		0, 1, 2,
		0, 2, 3,
		0, 4, 7,
		0, 7, 3,
		3, 7, 6,
		3, 6, 2,
		2, 6, 5,
		2, 5, 1,
		1, 5, 4,
		1, 4, 0,
		4, 5, 6,
		4, 6, 7
	};

	//Set up vertex array, vertex buffer, index buffer, shader, and texture
	VertexArray va;
	VertexBuffer vb(vertices, sizeof(vertices));
	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);
	layout.Push<float>(3);
	va.AddBuffer(vb, layout);
	IndexBuffer ib(indices, 6);
	Shader shader("shaders/default.vert", "shaders/default.frag");

	Texture diffTexture("obj/textures/planks.png");
	Texture specTexture("obj/textures/planksSpec.png");

	va.Unbind();
	vb.Unbind();
	ib.Unbind();
	shader.Unbind();
	diffTexture.Unbind();
	specTexture.Unbind();

	VertexArray lightVA;
	VertexBuffer lightVB(lightVertices, sizeof(lightVertices));
	VertexBufferLayout lightLayout;
	lightLayout.Push<float>(3);
	lightVA.AddBuffer(lightVB, lightLayout);
	IndexBuffer lightIB(lightIndices, 36);
	Shader lightShader("shaders/lighting.vert", "shaders/lighting.frag");

	lightVA.Unbind();
	lightVB.Unbind();
	lightIB.Unbind();
	lightShader.Unbind();

	Renderer renderer;
	Camera camera(glm::vec3(0.0f, 0.5f, 2.0f));

	glm::vec3 planePos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 planeModel = glm::mat4(1.0f);
	planeModel = glm::translate(planeModel, planePos);

	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
	glm::vec3 objectColor(1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Main loop
	while (!glfwWindowShouldClose(window)) {
		// Clear the screen
		renderer.Clear();

		camera.ProcessInput(window);

		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / HEIGHT, 0.1f, 100.0f);

		//pyramid
		shader.Bind();
		diffTexture.Bind(0);
		specTexture.Bind(1);
		va.Bind();
		shader.SetUniform1i("diffTexture", 0);
		shader.SetUniform1i("specTexture", 1);
		//shader.SetUniformVec3f("objectColor", objectColor);
		shader.SetUniformVec3f("lightColor", lightColor);
		shader.SetUniformVec3f("lightPos", lightPos);
		shader.SetUniformVec3f("viewPos", camera.GetPosition());
		shader.SetUniformMat4f("projection", projection);
		shader.SetUniformMat4f("view", view);
		shader.SetUniformMat4f("model", planeModel);
		renderer.Draw(va, ib, shader);

		//light source cube
		lightShader.Bind();
		lightVA.Bind();	
		lightShader.SetUniformVec3f("objectColor", objectColor);
		lightShader.SetUniformVec3f("lightColor", lightColor);
		lightShader.SetUniformMat4f("projection", projection);
		lightShader.SetUniformMat4f("model", lightModel);
		lightShader.SetUniformMat4f("view", view);
		renderer.Draw(lightVA, lightIB, lightShader);

		// Swap buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
	}

	// Clean up and exit
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
