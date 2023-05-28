#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

// Our own function to resize viewport.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	// --- Initializations ----------------------------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Create GLFW window
	GLFWwindow* window = glfwCreateWindow(800, 600, "NewOpenGL", NULL, NULL);
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
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	// --- Run-loop -----------------------------------------------------------
	while (!glfwWindowShouldClose(window))
	{
		// Process input
		processInput(window);

		// Rendering
		glClearColor(0.4f, 0.8f, 1.0f, 1);
		glClear(GL_COLOR_BUFFER_BIT); // Clears color bits. Can also clear depth

		// Events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	


	glfwTerminate();
	return 0;
}