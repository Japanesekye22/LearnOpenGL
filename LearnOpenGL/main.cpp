#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;



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



	// Vertices for a triangle
	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};
	// glGenBuffers creates memory in the GPU for a buffer.
	// sets the created buffer to the name VBO so we can access it.
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	// glBindBuffer sets buffer in the GPU to what you assign. In this case, 
	// it sets the array buffer in the GPU to VBO so we can modify it. 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData transfers the vertex data to the VBO. 
	// In this case, we transfer vertices to the array buffer in the GPU.
	// GL_STATIC_DRAW is the function type for performance. This for example
	// is for drawing a static object.
	// We dont need to specify VBO because VBO is the GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	


	//------------------------------Render Loop-----------------------------------
	while(!glfwWindowShouldClose(window))
	{
		// Input
		processInput(window);

		// Rendering commands

		// Sets the color when the color buffer is cleared.
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// Clears the color buffer.
		glClear(GL_COLOR_BUFFER_BIT);

		// Front render is what you see on screen. once the back render 
		// finishes rendering it swaps with the front render. this avoids 
		// artifacts if using one buffer.
		glfwSwapBuffers(window);
		// Checks for input events.
		glfwPollEvents();
	}

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
}