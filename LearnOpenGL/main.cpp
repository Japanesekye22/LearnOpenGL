#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Classes/shader.h"
#include "Classes/stb_image.h"
#include "Classes/camera.h"
#include "Classes/model.h"

#include <filesystem>
#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(char const* path);

const int SCR_WIDTH = 1920;
const int SCR_HEIGHT = 1080;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// deltaTime variables
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool rightPressed = false;

// World space location of light
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);



int main()
{
	//----------------------GLFW and GLAD initialization--------------------------
	// Initializes glfw
	glfwInit();
	// Tells glfw what version of opengl to use. this case: 4.6 Core
	const char* glsl_version = "#version 460";
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
	// Run every frame the mouse moves
	glfwSetCursorPosCallback(window, mouse_callback);
	// Run everytime scrollwheel is moved
	glfwSetScrollCallback(window, scroll_callback);

	// glad just calls openGL functions in an easier way
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	stbi_set_flip_vertically_on_load(true);


	//------------------------Main OpenGL Functions-------------------------------

	// Set viewport inside the window
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	// Enable depth buffer (Z index)
	glEnable(GL_DEPTH_TEST);
	// Hide cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	// Wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //GL_LINE

	// -------------Shaders---------------
	//
	Shader ourShader("Shaders/shader.vert", "Shaders/shader.frag");

	// --------------Model----------------
	Model ourModel("Models/backpack/backpack.obj");
	Model lightbulbModel("Models/lightbulb/lightbulb.obj");


	// --------------imgui----------------
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImGui::StyleColorsDark();



	//------------------------------Render Loop-----------------------------------
	while (!glfwWindowShouldClose(window))
	{
		
		// calculate deltaTime
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Input
		processInput(window);

		// Sets the color when the color buffer is cleared.
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffer

		// ----------------imgui------------------
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Demo window");
		ImGui::SliderFloat("translation", &lightPos.x, -5, 10);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		// -------------- Lighting ---------------
		//

		// activate shader
		ourShader.use();
		ourShader.setVec3("viewPos", camera.Position);
		ourShader.setFloat("material.shininess", 32.0f);

		// direction light
		ourShader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		ourShader.setVec3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		ourShader.setVec3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
		ourShader.setVec3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

		// point light
		ourShader.setVec3("pointLight.position", lightPos);
		ourShader.setVec3("pointLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		ourShader.setVec3("pointLight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		ourShader.setVec3("pointLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		ourShader.setFloat("pointLight.constant", 1.0f);
		ourShader.setFloat("pointLight.linear", 0.09f);
		ourShader.setFloat("pointLight.quadratic", 0.032f);

		// ----------- Transformations -----------
		// 
		glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		if (rightPressed)
		{
			glm::mat4 view = camera.GetViewMatrix();
			ourShader.setMat4("view", view);
		}
		ourShader.setMat4("projection", projection);
		

		// draw main model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		ourShader.setMat4("model", model);
		ourModel.Draw(ourShader);

		// draw lightbulb model
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		ourShader.setMat4("model", model);
		lightbulbModel.Draw(ourShader);

		glfwSwapBuffers(window);
		// Checks for input events.
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
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
	// Movement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		rightPressed = true;
	}
		
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		rightPressed = false;
	}
		

	// Escape to close window.
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

// Callback function for mouse input.
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	// Prevent frame skip on first mouse input
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	// Calculate offset from last mouse position
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// dont really need since model object does this
unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
		{
			format = GL_RED;
		}
		else if (nrComponents == 3)
		{
			format = GL_RGB;
		}
		else if (nrComponents == 4)
		{
			format = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}