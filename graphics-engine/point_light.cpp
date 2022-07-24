#include "point_light.h"

PointLight::PointLight(glm::vec3 color, glm::vec3 position)
{
	diffuseColor = color * glm::vec3(0.5f);
	ambientColor = diffuseColor * glm::vec3(0.5f);
	this->position = position;
}