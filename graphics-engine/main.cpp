#include <iostream>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include "shader.h"
#include "camera.h"
#include "model.h"
#include "object.h"
#include "scene.h"
#include "plane.h"
#include "cube.h"
#include "sphere.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

GLFWwindow* createWindow(const char* title, const unsigned int width = WINDOW_WIDTH, const unsigned int height = WINDOW_HEIGHT);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void processInput(GLFWwindow* window);
double calculateFramerate();

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera(glm::vec3(0.0f, 1.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -20.0f);
float lastX = WINDOW_WIDTH / 2.0f;
float lastY = WINDOW_HEIGHT / 2.0f;

int main()
{
	printf("Program starting...\n");
	try
	{
		GLFWwindow* window = createWindow("Interesting Title");
		glfwSetFramebufferSizeCallback(window, framebufferSizeCallback); //Change viewport when window size is changed
		glfwSetCursorPosCallback(window, mouseCallback);
		glfwSetScrollCallback(window, scrollCallback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_CULL_FACE);

		Shader shader("shader.vert", "shader.frag");
		Shader constantColorShader("shader.vert", "constant-color.frag");

		Cube light(constantColorShader,
			glm::vec3(0.7f, 0.2f, 2.0f),
			glm::vec3(0.1f)
		);
		DirectionalLight directionalLight(glm::vec3(1.0f), glm::vec3(1.0f, -1.0f, -1.0f));
		PointLight pointLight(glm::vec3(1.0f), light.getPosition());
		light.pointLights = std::vector({pointLight});
		light.directionalLights = std::vector({directionalLight});

		Cube greenLight(constantColorShader, glm::vec3(-1.0f, 1.0f, -10.0f));
		PointLight greenPointLight(glm::vec3(0.0f, 1.0f, 0.0f), greenLight.getPosition());
		greenLight.pointLights = std::vector({pointLight});


		Plane plane(shader, glm::vec3(0.0f), glm::vec3(10.0f));
		Cube cube(shader, glm::vec3(0.0f, 1.0f, 0.0f));
		Sphere manyPoints(shader, 100, glm::vec3(2.0f, 1.0f, 0.0f));
		Sphere fewPoints(shader, 5, glm::vec3(4.0f, 1.0f, 0.0f));
		Sphere evenFewerPoints(shader, 3, glm::vec3(6.0f, 1.0f, 0.0f));

		Scene scene(shader, camera);
		scene.addObjects({light, plane, cube, manyPoints, fewPoints, evenFewerPoints});
		
		//Rendering loop
		while (!glfwWindowShouldClose(window))
		{
			glfwSetWindowTitle(window, ("Framerate: " + std::to_string(calculateFramerate())).c_str());

			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			processInput(window);

			glClearColor(0.1f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			glStencilMask(0x00);

			scene.draw(window);

			glBindVertexArray(0);
			
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		shader.del();
		constantColorShader.del();

		glfwTerminate();
		return 0;
	}
	catch (int e)
	{
		std::cout << "ERROR" << std::endl;
		glfwTerminate();
		return e;
	}
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xPosIn, double yPosIn)
{
	float xPos = static_cast<float>(xPosIn);
	float yPos = static_cast<float>(yPosIn);

	static bool firstMouse = true;
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	camera.processMouseMovement(xOffset, yOffset);
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.processMouseScroll(yOffset);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	const float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.processKeyboard(Camera::Movement::FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.processKeyboard(Camera::Movement::BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.processKeyboard(Camera::Movement::LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.processKeyboard(Camera::Movement::RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		camera.processKeyboard(Camera::Movement::UP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		camera.processKeyboard(Camera::Movement::DOWN, deltaTime);
	}
}

GLFWwindow* createWindow(const char* title, const unsigned int width, const unsigned int height)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create window
	GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		throw - 1;
	}

	glfwMakeContextCurrent(window);

	//Initialise GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialise GLAD" << std::endl;
		throw - 1;
	}

	//Set viewport 
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	return window;
}

double calculateFramerate()
{
	static double lastTime = glfwGetTime();
	static int framesCount = 0;
	static double frameRate = 0.0f;

	double currentTime = glfwGetTime();
	framesCount++;

	if (currentTime - lastTime >= 1.0f)
	{
		frameRate = framesCount / (currentTime - lastTime);
		framesCount = 0;
		lastTime = currentTime;
	}

	return frameRate;
}