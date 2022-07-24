#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include <glm/glm.hpp>

class DirectionalLight
{
public:
	DirectionalLight(glm::vec3 color, glm::vec3 direction);

	glm::vec3 direction;
	glm::vec3 diffuseColor;
	glm::vec3 ambientColor;
};

#endif