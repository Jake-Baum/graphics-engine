#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "shader.h"

class PointLight
{
public:
	PointLight(glm::vec3 color, glm::vec3 position, Shader& lightingShader);

	void setLightUniforms();

private:
	static int numberOfLights;
	static int currentLight;

	Shader* lightingShader;
	glm::vec3 position;
	glm::vec3 diffuseColor;
	glm::vec3 ambientColor;
};
#endif