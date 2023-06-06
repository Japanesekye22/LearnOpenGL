#include <iostream>

#define STB_IMAGE_IMPLEMENTATION

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

float mixValue = 0.2f;

// Our own function to resize viewport.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		if (mixValue < 1)
			mixValue += 0.001f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		if (mixValue > 0)
			mixValue -= 0.001f;
}

int main()
{
	// --- Initializations ----------------------------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Create GLFW window
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "NewOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Load glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	// Set opengl context within GLFW window. Origin lower left
	glViewport(0, 0, 1920, 1080);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// --- Render Triangle ----------------------------------------------------
	
	// --- Vertex Array ---
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// --- Vertex Buffer ---
	float vertices[] =
	{
		// positions        // colors         // texture
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
	};
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind VBO to array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Link data with VBO

	// --- Index Buffer ---
	unsigned int indices[] =
	{
		0, 1, 2, 0, 2, 3
	};
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// --- layout --- 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// --- Shaders ---
	Shader ourShader("src/Shaders/vertexShader.vs", "src/Shaders/fragmentShader.fs");
	ourShader.use();

	// --- Textures ---
	float texCoords[] = 
	{
		0.0f, 0.0f, // lower-left corner
		1.0f, 0.0f, // lower-right corner
		0.5f, 1.0f // top-center corner
	};
	// Sets texture behavior when texture coords exceed -1, 1
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// Sets texture filtering. 
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	stbi_set_flip_vertically_on_load(true);
	// Texture 1
	int width, height, nrChannels;
	unsigned char* data = stbi_load("resources/container.jpg", &width, &height, &nrChannels, 0);
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	// Texture 2
	data = stbi_load("resources/awesomeface.png", &width, &height, &nrChannels, 0);
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);



	// --- Run-loop -----------------------------------------------------------
	while (!glfwWindowShouldClose(window))
	{
		// Process input
		processInput(window);

		// Rendering
		glClearColor(0.3f, 0.3f, 0.4f, 1);
		glClear(GL_COLOR_BUFFER_BIT); // Clears color bits. Can also clear depth

		// Create an identity matrix first. Then apply transformations
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
		trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

		unsigned int transformLocation = glGetUniformLocation(ourShader.getID(), "transform");
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(trans));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		ourShader.setFloat("mixValue", mixValue);
		ourShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}