#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

// Shader for vertex data.
const char *vertexShaderSource = "#version 460 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
	"}\0";
// Shader for color data.
const char *fragmentShaderSource = "#version 460 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	" FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";



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


	// -----------Vertex Shader------------
	//
	// Create a vertex shader, attach source code, then compile.
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Check if compilation was successful.
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}


	// -----------Fragment Shader------------
	//
	// Create a fragment shader object, attach source code, then compile.
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Check if compilation was successful.
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::FRAGMENT::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}


	// ----------Link Shaders--------------
	//
	// Create shader program object.
	unsigned int shaderProgram = glCreateProgram();
	// Attach the shaders to program, then link them together.
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check if linking successful.
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
			infoLog << std::endl;
	}
	// delete shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	// -----------Vertex Input------------
	// 
	// Vertices for a triangle
	float vertices[] =
	{
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};
	unsigned int indices[] =
	{
		0, 1, 3,
		1, 2, 3
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// Enable vertex attribute.
	glEnableVertexAttribArray(0);
	


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

		// Draw triangle
		glUseProgram(shaderProgram);
		// Since we only have one VAO, no need to bind every frame.
		glBindVertexArray(VAO);
		// Draws from vertex array. Primitive, starting index, how many vertices.
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
	glDeleteProgram(shaderProgram);

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