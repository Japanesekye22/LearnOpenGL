#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);


int main()
{
	//initializes glfw
	glfwInit();
	//tells glfw what version of opengl to use. this case: 4.6 Core
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create GLFW object. Inlcuding Error checking.
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//use the object window as the current window.
	glfwMakeContextCurrent(window);
	//function to change window size when dragging window.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//set viewport inside the window
	glViewport(0, 0, 800, 600);

	//render loop
	while(!glfwWindowShouldClose(window))
	{
		//front render is what you see on screen. once the back render 
		//finishes rendering it swaps with the front render. this avoids 
		//artifacts if using one buffer.
		glfwSwapBuffers(window);
		//checks for input events.
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

//function that sets internal viewport of window to the window size when
//resizing window.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}