#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "shader.h"

class DirectionalLight
{
public:
	DirectionalLight(glm::vec3 color, glm::vec3 direction, Shader& lightingShader);
	~DirectionalLight();

	void setLightUniforms();

private:
	static int numberOfLights;
	static int currentLight;

	Shader* lightingShader;
	glm::vec3 direction;
	glm::vec3 diffuseColor;
	glm::vec3 ambientColor;
};

#endif