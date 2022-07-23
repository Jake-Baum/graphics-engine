#include "directional_light.h"

int DirectionalLight::numberOfLights = 0;
int DirectionalLight::currentLight = 0;

DirectionalLight::DirectionalLight(glm::vec3 color, glm::vec3 direction, Shader& lightingShader)
{
	numberOfLights++;
	diffuseColor = color * glm::vec3(0.5f);
	ambientColor = diffuseColor * glm::vec3(0.5f);
	this->direction = direction;
	this->lightingShader = &lightingShader;
}

DirectionalLight::~DirectionalLight()
{
	numberOfLights--;
}

void DirectionalLight::setLightUniforms()
{
	if (currentLight >= 8)
	{
		throw -1;
	}

	lightingShader->use();

	lightingShader->setInt("numberOfDirectionalLights", currentLight + 1);
	lightingShader->setVec3("directionalLights[" + std::to_string(currentLight) + "].direction", glm::vec3(1.0f, -1.0f, -1.0f));
	lightingShader->setVec3("directionalLights[" + std::to_string(currentLight) + "].ambient", ambientColor);
	lightingShader->setVec3("directionalLights[" + std::to_string(currentLight) + "].diffuse", diffuseColor);
	lightingShader->setVec3("directionalLights[" + std::to_string(currentLight) + "].specular", glm::vec3(1.0f));

	currentLight++;
	if (currentLight >= numberOfLights)
	{
		currentLight = 0;
	}
}
