#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Classes/shader.h"
#include "Classes/stb_image.h"

#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

float mixValue = 0.2f;


int main()
{
	//----------------------GLFW and GLAD initialization--------------------------
	// Initializes glfw
	glfwInit();
	// Tells glfw what version of opengl to use. this case: 4.6 Core
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create GLFW object. Inlcuding Error checking.
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Use the object window as the current window.
	glfwMakeContextCurrent(window);
	// Function to change window size when dragging window.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad just calls openGL functions in an easier way
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	//------------------------Main OpenGL Functions-------------------------------

	// Set viewport inside the window
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	// -------------Shaders---------------
	//
	Shader ourShader("Classes/shader.vs", "Classes/shader.fs");

	// -----------Vertex Input------------
	// 
	// Vertices for a triangle.
	float vertices[] =
	{
		 // position            // texture
		 -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
		  0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
		  0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
		  0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
		 -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
		 -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,

		 -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
		  0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
		  0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
		  0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
		 -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,
		 -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,

		 -0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
		 -0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
		 -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
		 -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
		 -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
		 -0.5f,  0.5f,  0.5f,   1.0f, 0.0f,

		  0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
		  0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
		  0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
		  0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
		  0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
		  0.5f,  0.5f,  0.5f,   1.0f, 0.0f,

		 -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
		  0.5f, -0.5f, -0.5f,   1.0f, 1.0f,
		  0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
		  0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
		 -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
		 -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,

		 -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
		  0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
		  0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
		  0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
		 -0.5f,  0.5f,  0.5f,   0.0f, 0.0f,
		 -0.5f,  0.5f, -0.5f,   0.0f, 1.0f

	};
	// Order to connect vertices.
	unsigned int indices[] =
	{
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	glm::vec3 cubePositions[] =
	{
		glm::vec3( 0.0f, -1.5f,  0.0f),
		glm::vec3( 2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3( 2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3( 1.3f, -2.0f, -2.5f),
		glm::vec3( 1.5f,  2.0f, -2.5f),
		glm::vec3( 1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)

	};
	// Generate buffers and vertex array.
	unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	// Bind buffers and vertex array to the GPU's elements.
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// Add values into the buffers.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Tell OpenGL how to interpret vertex data.
	// Attribute position, size of attribute (Ex. vec3 is 3), type of data,
	// whether to normalize data (0 to 1), stride: size of each vertex data, 
	// offset of where position data begins.

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	// ----------- Texture 1 ------------
	// 
	// Generate texture object
	unsigned int texture1;
	// Tell OpenGL to flip textures.
	stbi_set_flip_vertically_on_load(true);

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1); // Now all GL_TEXTURE_2D operations references texture.
	// Define wrapping mode. 2D, S axis (texture is str not xyz), wrapping type outside 0 or 1.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	
	// Define filter mode. Texture downscaled = nearest (no filter), upscaled = linear.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// Load image
	int width, height, nrChannels; // nrChannels = number of color channels.
	unsigned char* data = stbi_load("Images/container.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{
		// Generate textures and mipmap
		// Target, mipmap level, color data, width, height, 0, Image format, data type, actual image.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// ----------- Texture 2 ----------
	//
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	data = stbi_load("Images/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// Tell shader which sampler goes with which texture unit
	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);

	// Enable depth buffer (Z index)
	glEnable(GL_DEPTH_TEST);


	//------------------------------Render Loop-----------------------------------
	while (!glfwWindowShouldClose(window))
	{
		// Input
		processInput(window);

		// Input change mix value
		ourShader.setFloat("mixValue", mixValue);

		// Sets the color when the color buffer is cleared.
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffer

		// Bind textures to texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// Activate shader program
		ourShader.use();


		// ----------- Transformations -----------
		// 
		// Create model, view, and projection matrix
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		// Set uniform for matrices
		view = glm::translate(view, glm::vec3(1.0f, 0.0f, -3.0f)); // moves away.
		projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

		// Set uniforms for the transformation matrices	
		ourShader.setMat4("view", view);
		ourShader.setMat4("projection", projection); // Better to set outside of loop



		// Since we only have one VAO, no need to bind every frame.
		glBindVertexArray(VAO);
		// Draw cubes.
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader.setMat4("model", model);
			
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		// Front render is what you see on screen. once the back render 
		// finishes rendering it swaps with the front render. this avoids 
		// artifacts if using one buffer.
		glfwSwapBuffers(window);
		// Checks for input events.
		glfwPollEvents();
	}

	// deallocate all resources once closed.
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}

// Sets OpenGL viewport size to GLFW window size when resizing.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Input function to organize all the inputs.
void processInput(GLFWwindow* window)
{
	// Escape to close window.
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mixValue += 0.01f;
		if (mixValue >= 1.0f)
		{
			mixValue = 1.0f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mixValue -= 0.01f;
		if (mixValue <= 0.0f)
		{
			mixValue = 0.0f;
		}
	}
}