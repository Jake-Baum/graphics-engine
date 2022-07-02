#include "shader.h"

unsigned int Shader::compileShader(const char* shaderSource, GLenum type)
{
	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);

	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[INFO_LOG_MAX_LENGTH];
		glGetShaderInfoLog(shader, INFO_LOG_MAX_LENGTH, NULL, infoLog);
		std::cout << "Shader failed to compile: " << infoLog << std::endl;
		throw - 1;
	}

	return shader;
}

unsigned int Shader::createShaderProgram(const std::vector<unsigned int> shaders)
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
		char infoLog[INFO_LOG_MAX_LENGTH];
		glGetProgramInfoLog(shaderProgram, INFO_LOG_MAX_LENGTH, NULL, infoLog);
		std::cout << "Failed to link shaders: " << infoLog << std::endl;
		throw - 1;
	}

	return shaderProgram;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode, fragmentCode;
	std::ifstream vertexShaderFile, fragmentShaderFile;

	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		std::stringstream vertexShaderStream, fragmentShaderStream;
		vertexShaderFile.open(vertexPath);
		vertexShaderStream << vertexShaderFile.rdbuf();
		vertexShaderFile.close();
		vertexCode = vertexShaderStream.str();

		fragmentShaderFile.open(fragmentPath);
		fragmentShaderStream << fragmentShaderFile.rdbuf();
		fragmentShaderFile.close();
		fragmentCode = fragmentShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "Shader files not read successfully: " << e.what() << std::endl;
	}

	const char* vertexShaderCode = vertexCode.c_str();
	const char* fragmentShaderCode = fragmentCode.c_str();

	unsigned int vertex = compileShader(vertexShaderCode, GL_VERTEX_SHADER),
		fragment = compileShader(fragmentShaderCode, GL_FRAGMENT_SHADER);

	id = createShaderProgram({vertex, fragment});

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use()
{
	glUseProgram(id);
}

void Shader::del()
{
	glDeleteProgram(id);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, glm::mat4 matrix) const
{
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setVec3(const std::string& name, glm::vec3 vector) const
{
	glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(vector));
}

void Shader::setVec4(const std::string& name, glm::vec4 vector) const
{
	glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(vector));
}
