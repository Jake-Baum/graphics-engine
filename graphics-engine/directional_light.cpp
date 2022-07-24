#include "directional_light.h"

DirectionalLight::DirectionalLight(glm::vec3 color, glm::vec3 direction)
{
	diffuseColor = color * glm::vec3(0.5f);
	ambientColor = diffuseColor * glm::vec3(0.5f);
	this->direction = direction;
}
