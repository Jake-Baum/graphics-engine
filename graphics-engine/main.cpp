#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
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

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const std::vector<Vertex> CUBE_VERTICES = {
	{glm::vec3(-0.5f, -0.5f, 0.5f), glm::normalize(glm::vec3(-1.0f, -1.0f, 1.0f)), glm::vec2()},
	{glm::vec3(-0.5f, 0.5f, 0.5f), glm::normalize(glm::vec3(-1.0f, 1.0f, 1.0f)), glm::vec2()},
	{glm::vec3(0.5f, 0.5f, 0.5f), glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)), glm::vec2()},
	{glm::vec3(0.5f, -0.5f, 0.5f), glm::normalize(glm::vec3(1.0f, -1.0f, 1.0f)), glm::vec2()},
	{glm::vec3(-0.5f, -0.5f, -0.5f), glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)), glm::vec2()},
	{glm::vec3(-0.5f, 0.5f, -0.5f), glm::normalize(glm::vec3(-1.0f, 1.0f, -1.0f)), glm::vec2()},
	{glm::vec3(0.5f, 0.5f, -0.5f), glm::normalize(glm::vec3(1.0f, 1.0f, -1.0f)), glm::vec2()},
	{glm::vec3(0.5f, -0.5f, -0.5f), glm::normalize(glm::vec3(1.0f, -1.0f, -1.0f)), glm::vec2()}
};

const std::vector<unsigned int> CUBE_INDICES = {0, 1, 2, 0, 2, 3, 4, 5, 1, 4, 1, 0, 7, 6, 5, 7, 5, 4, 3, 2, 6, 3, 6, 7, 4, 0, 3, 4, 3, 7, 1, 5, 6, 1, 6, 2};

const std::vector<Vertex> PLANE_VERTICES = {
	{glm::vec3(-0.5f, 0.0f, 0.5f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
	{glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
	{glm::vec3( 0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
	{glm::vec3( 0.5f, 0.0f, 0.5f),  glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)}
};

const std::vector<unsigned int> PLANE_INDICES = {0, 1, 2, 0, 2, 3};

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

		Shader shader("shader.vert", "shader.frag");
		Shader constantColorShader("shader.vert", "constant-color.frag");

		Model cube = Model(
			Mesh(
				std::vector<Vertex>({CUBE_VERTICES}),
				std::vector<unsigned int>(CUBE_INDICES),
				std::vector<Texture>()
			)
		);
		Object light = Object(
			cube,
			constantColorShader,
			glm::vec3(0.7f, 0.2f, 2.0f),
			glm::vec3(0.1f)
		);
		DirectionalLight directionalLight = DirectionalLight(glm::vec3(1.0f), glm::vec3(1.0f, -1.0f, -1.0f), shader);
		PointLight pointLight = PointLight(glm::vec3(1.0f), light.getPosition(), shader);
		light.pointLights = std::vector({pointLight});
		light.directionalLights = std::vector({directionalLight});

		std::cout << "Loading model..." << std::endl;
		Object backpack = Object(Model("backpack.obj"), shader, glm::vec3(0.0f, 0.5f, -1.0f), glm::vec3(0.2f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f), 64.0f);
		backpack.outlineShader = std::optional(&constantColorShader);
		std::cout << "Model Loaded" << std::endl;

		Model plane = Model(
			Mesh(
				std::vector<Vertex>({PLANE_VERTICES}),
				std::vector<unsigned int>(PLANE_INDICES),
				std::vector<Texture>({
					{
						Model::constantColorTexture(glm::vec4(128.0f), glm::vec2(644.0f)),
						Mesh::TEXTURE_SPECULAR,
						"grey.png"
					},
					{
						Model::constantColorTexture(glm::vec4(200.0f), glm::vec2(64.0f)),
						Mesh::TEXTURE_SPECULAR,
						"ligh-grey.png"
					}
				})
			)
		);
		Object floor = Object(
			plane,
			shader,
			glm::vec3(0.0f),
			glm::vec3(10.0f)
		);

		Model grass = Model(
			Mesh(
				std::vector<Vertex>({PLANE_VERTICES}),
				std::vector<unsigned int>(PLANE_INDICES),
				std::vector<Texture>({
					{
						Model::textureFromFile("grass.png"),
						Mesh::TEXTURE_DIFFUSE,
						"grass.png"
					},
					{
						Model::constantColorTexture(glm::vec4(0.0f), glm::vec2(64.0f)),
						Mesh::TEXTURE_SPECULAR,
						"black.png"
					}
				})
			)
		);

		Model transparentWindow = Model(
			Mesh(
				std::vector<Vertex>({PLANE_VERTICES}),
				std::vector<unsigned int>(PLANE_INDICES),
				std::vector<Texture>({
					{
						Model::textureFromFile("blending_transparent_window.png"),
						Mesh::TEXTURE_DIFFUSE,
						"blending_transparent_window.png"
					},
					{
						Model::constantColorTexture(glm::vec4(200.0f), glm::vec2(64.0f)),
						Mesh::TEXTURE_SPECULAR,
						"light-grey.png"
					}
				})
			)
		);

		std::vector<Object> windows = std::vector<Object>(
			{
				Object(transparentWindow, shader, glm::vec3(1.5f, 0.5f, 0.51f), glm::vec3(1.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f), 500.0f),
				Object(transparentWindow, shader, glm::vec3(0.5f, 0.5f, -0.6f), glm::vec3(1.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f), 500.0f),
				Object(transparentWindow, shader, glm::vec3(-1.5f, 0.5f, -0.48f), glm::vec3(1.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f), 500.0f),
				Object(transparentWindow, shader, glm::vec3(-0.3f, 0.5f, -2.3f), glm::vec3(1.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f), 500.0f),
				Object(transparentWindow, shader, glm::vec3(0.0f, 0.5f, 0.7f), glm::vec3(1.0f), 90.0f, glm::vec3(1.0f, 0.0f, 0.0f), 500.0f),
			}
		);

		std::vector<Object> objects = std::vector<Object>(
			{
				backpack, floor
			}
		);
		objects.insert(objects.end(), windows.begin(), windows.end());
		
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

			//Set projection and view
			glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);

			constantColorShader.use();
			constantColorShader.setMat4("view", camera.getViewMatrix());
			constantColorShader.setMat4("projection", projection);

			shader.use();
			shader.setMat4("projection", projection);
			shader.setMat4("view", camera.getViewMatrix());
			shader.setVec3("viewPosition", camera.position);
			
			//Draw point light
			constantColorShader.use();
			constantColorShader.setVec4("color", glm::vec4(1.0f));
			light.draw();

			//Draw objects
			std::map<float, Object> sorted;
			for (auto& object : objects)
			{
				float distance = glm::length(camera.position - object.getPosition());
				sorted.insert(std::make_pair(distance, object));
			}
			for (std::map<float, Object>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
			{
				it->second.draw();
			}

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