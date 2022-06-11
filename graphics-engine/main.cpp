#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const char* vertexShaderSource = R"(
	#version 460 core

	layout (location = 0) in vec3 aPos;

	void main() 
	{
		gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	}
)";

const char* fragmentShaderSource = R"(
	#version 460 core
	
	out vec4 fragColor;

	void main()
	{
		fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	}
)";

const char* fragmentShaderSource2 = R"(
	#version 460 core
	
	out vec4 fragColor;

	void main()
	{
		fragColor = vec4(1.0f, 1.0f, 0.2f, 1.0f);
	}
)";

GLFWwindow* createWindow(const char* title, const unsigned int width = WINDOW_WIDTH, const unsigned int height = WINDOW_HEIGHT);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
unsigned int compileShader(const char* shaderSource, GLenum type);
unsigned int createShaderProgram(const std::vector<unsigned int> shaders);

int main()
{
	try
	{
		GLFWwindow* window = createWindow("Interesting Title");

		std::vector<unsigned int> shaders = {compileShader(vertexShaderSource, GL_VERTEX_SHADER), compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER)};
		unsigned int shaderPrograms[2];
		shaderPrograms[0] = createShaderProgram(shaders);

		//Delete individual shaders as unnecessary after linking
		for (unsigned int shader : shaders)
		{
			glDeleteShader(shader);
		}

		shaders = {compileShader(vertexShaderSource, GL_VERTEX_SHADER), compileShader(fragmentShaderSource2, GL_FRAGMENT_SHADER)};
		shaderPrograms[1] = createShaderProgram(shaders);

		//Delete individual shaders as unnecessary after linking
		for (unsigned int shader : shaders)
		{
			glDeleteShader(shader);
		}

		float leftRectangle[] = {
			-0.1f,  0.5f, 0.0f,  // top right
			-0.1f, -0.5f, 0.0f,  // bottom right
			-0.5f, -0.5f, 0.0f,  // bottom left
			-0.5f,  0.5f, 0.0f   // top left 
		};
		float rightRectangle[] = {
			 0.5f,  0.5f, 0.0f,  // top right
			 0.5f, -0.5f, 0.0f,  // bottom right
			 0.1f, -0.5f, 0.0f,  // bottom left
			 0.1f,  0.5f, 0.0f   // top left 
		};

		unsigned int indices[] = {
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};


		unsigned int vbos[2], vaos[2], ebo;
		glGenVertexArrays(2, vaos);
		glGenBuffers(2, vbos);
		glGenBuffers(1, &ebo);

		//Left rectangle
		glBindVertexArray(vbos[0]);
		glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(leftRectangle), leftRectangle, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		//Right rectangle
		glBindVertexArray(vbos[1]);
		glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(rightRectangle), rightRectangle, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		//Rendering loop
		while (!glfwWindowShouldClose(window))
		{
			processInput(window);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);


			for (unsigned int i = 0; i < 2; i++)
			{
				glUseProgram(shaderPrograms[i]);
				glBindVertexArray(vaos[i]);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}

			glBindVertexArray(0);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glDeleteVertexArrays(2, vaos);
		glDeleteBuffers(2, vbos);
		for (unsigned int shaderProgram : shaderPrograms)
		{
			glDeleteProgram(shaderProgram);
		}

		glfwTerminate();
		return 0;
	}
	catch (int e)
	{
		glfwTerminate();
		return e;
	}
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

GLFWwindow* createWindow(const char *title, const unsigned int width, const unsigned int height)
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
		throw -1;
	}

	glfwMakeContextCurrent(window);

	//Initialise GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialise GLAD" << std::endl;
		throw -1;
	}

	//Set viewport 
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback); //Change viewport when window size is changed

	return window;
}

unsigned int compileShader(const char* shaderSource, GLenum type)
{
	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);

	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "Fragment shader 2 failed to compile: " << infoLog << std::endl;
		throw - 1;
	}

	return shader;
}

unsigned int createShaderProgram(const std::vector<unsigned int> shaders)
{

	//Create shader program, combining vertex and fragment shaders
	unsigned int shaderProgram = glCreateProgram();

	for (unsigned int shader : shaders)
	{
		glAttachShader(shaderProgram, shader);
	}
	glLinkProgram(shaderProgram);

	int success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Failed to link shaders: " << infoLog << std::endl;
		throw - 1;
	}

	return shaderProgram;
}