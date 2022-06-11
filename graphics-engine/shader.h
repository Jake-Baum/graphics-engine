#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

class Shader
{
private:
	static const unsigned int INFO_LOG_MAX_LENGTH = 512;
	static unsigned int compileShader(const char* shaderSource, GLenum type);
	static unsigned int createShaderProgram(const std::vector<unsigned int> shaders);

public:
	unsigned int id;

	Shader(const char* vertexPath, const char* fragmentPath);

	void use();
	void del();

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
};

#endif