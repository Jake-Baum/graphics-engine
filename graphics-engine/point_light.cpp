#include "point_light.h"

int PointLight::numberOfLights = 0;
int PointLight::currentLight = 0;

PointLight::PointLight(glm::vec3 color, glm::vec3 position, Shader& lightingShader)
{
	numberOfLights++;
	diffuseColor = color * glm::vec3(0.5f);
	ambientColor = diffuseColor * glm::vec3(0.5f);
	this->position = position;
	this->lightingShader = &lightingShader;
}

void PointLight::setLightUniforms()
{
	lightingShader->use();

	lightingShader->setVec3("pointLights[" + std::to_string(currentLight) + "].position", position);
	lightingShader->setFloat("pointLights[" + std::to_string(currentLight) + "].constant", 1.0f);
	lightingShader->setFloat("pointLights[" + std::to_string(currentLight) + "].linear", 0.1f);
	lightingShader->setFloat("pointLights[" + std::to_string(currentLight) + "].quadratic", 0.01f);
	lightingShader->setVec3("pointLights[" + std::to_string(currentLight) + "].ambient", ambientColor);
	lightingShader->setVec3("pointLights[" + std::to_string(currentLight) + "].diffuse", diffuseColor);
	lightingShader->setVec3("pointLights[" + std::to_string(currentLight) + "].specular", glm::vec3(1.0f));

	currentLight++;
	if (currentLight >= numberOfLights)
	{
		currentLight = 0;
	}
}
