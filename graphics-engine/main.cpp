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

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const float CUBE_VERTICES[] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
};

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

		unsigned int vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(CUBE_VERTICES), CUBE_VERTICES, GL_STATIC_DRAW);

		unsigned int lightVao;
		glGenVertexArrays(1, &lightVao);
		glBindVertexArray(lightVao);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glm::vec3 pointLightPositions[] = {
			glm::vec3(0.7f,  0.2f,  2.0f)
		};

		std::cout << "Loading model..." << std::endl;
		Model backpack = Model("backpack.obj");
		std::cout << "Model Loaded" << std::endl;

		Model plane = Model(
			Mesh(
				std::vector<Vertex>({PLANE_VERTICES}),
				std::vector<unsigned int>(PLANE_INDICES),
				std::vector<Texture>()
			)
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
						"black.png"
					}
				})
			)
		);

		std::vector<glm::vec3> vegetation = std::vector<glm::vec3>(
			{
				glm::vec3( 1.5f, 0.5f,  0.51f),
				glm::vec3( 0.5f, 0.5f, -0.6f),
				glm::vec3(-1.5f, 0.5f, -0.48f),
				glm::vec3(-0.3f, 0.5f, -2.3f),
				glm::vec3( 0.0f, 0.5f,  0.7f),
			}
		);
		
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

			glm::mat4 model;
			glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f);

			//Draw point light
			glBindVertexArray(lightVao);
			constantColorShader.use();
			constantColorShader.setVec4("color", glm::vec4(1.0f));
			for (glm::vec3 pointLightPosition : pointLightPositions)
			{
				model = glm::mat4(1.0f);
				model = glm::translate(model, pointLightPosition);
				model = glm::scale(model, glm::vec3(0.1f));
				constantColorShader.setMat4("model", model);
				constantColorShader.setMat4("view", camera.getViewMatrix());
				constantColorShader.setMat4("projection", projection);

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

			//Draw plane
			model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(10.0f));
			constantColorShader.setVec4("color", glm::vec4(0.5f));
			constantColorShader.setMat4("model", model);
			plane.draw(constantColorShader);


			//Set up lighting
			shader.use();

			glm::vec3 lightColor = glm::vec3(1.0f);
			glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
			glm::vec3 ambientColor = diffuseColor * glm::vec3(0.5f);

			shader.setVec3("directionalLight.direction", glm::vec3(1.0f, -1.0f, -1.0f));
			shader.setVec3("directionalLight.ambient", ambientColor);
			shader.setVec3("directionalLight.diffuse", diffuseColor);
			shader.setVec3("directionalLight.specular", glm::vec3(1.0f));

			for (int i = 0; i < sizeof(pointLightPositions) / sizeof(pointLightPositions[0]); i++)
			{
				shader.setVec3("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
				shader.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
				shader.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.1f);
				shader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.01f);
				shader.setVec3("pointLights[" + std::to_string(i) + "].ambient", ambientColor);
				shader.setVec3("pointLights[" + std::to_string(i) + "].diffuse", diffuseColor);
				shader.setVec3("pointLights[" + std::to_string(i) + "].specular", glm::vec3(1.0f));
			}

			//Draw backpack
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);
			model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(0.2f));
			model = glm::translate(model, glm::vec3(0.0f, 2.0f, -5.0f));
			shader.setMat4("model", model);
			shader.setMat4("projection", projection);
			shader.setMat4("view", camera.getViewMatrix());

			shader.setVec3("viewPosition", camera.position);
			
			shader.setFloat("material.shininess", 64.0f);

			backpack.draw(shader);

			//Draw outline
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);
			constantColorShader.use();
			constantColorShader.setVec4("color", glm::vec4(0.75f, 0.75f, 0.2f, 1.0f));

			model = glm::scale(model, glm::vec3(1.01f));
			constantColorShader.setMat4("model", model);
			constantColorShader.setMat4("projection", projection);
			constantColorShader.setMat4("view", camera.getViewMatrix());
			backpack.draw(constantColorShader);

			glStencilMask(0xFF);
			glStencilFunc(GL_ALWAYS, 0, 0xFF);
			
			glClear(GL_STENCIL_BUFFER_BIT);
			glStencilMask(0x00);

			shader.use();

			//Draw windows
			std::map<float, glm::vec3> sorted;
			for (auto& position : vegetation)
			{
				float distance = glm::length(camera.position - position);
				sorted[distance] = position;
			}

			for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
			{
				model = glm::mat4(1.0f);
				model = glm::translate(model, it->second);
				model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

				shader.setMat4("model", model);
				shader.setMat4("projection", projection);
				shader.setMat4("view", camera.getViewMatrix());

				shader.setVec3("viewPosition", camera.position);

				shader.setFloat("material.shininess", 500.0f);

				transparentWindow.draw(shader);
			}

			glBindVertexArray(0);
			
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glDeleteBuffers(1, &vbo);
		shader.del();

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