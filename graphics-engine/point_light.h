#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include <glm/glm.hpp>

class PointLight
{
public:
	PointLight(glm::vec3 color, glm::vec3 position);

	glm::vec3 position;
	glm::vec3 diffuseColor;
	glm::vec3 ambientColor;
};
#endif