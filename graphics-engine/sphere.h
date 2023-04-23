#pragma once
#include "object.h"

#define _USE_MATH_DEFINES
#include <math.h>

class Sphere : public Object
{
public:
	Sphere(Shader& shader,
				 unsigned int stacksNumber,
				 unsigned int slicesNumber,
				 glm::vec3 position = glm::vec3(0.0f),
				 glm::vec3 scale = glm::vec3(1.0f),
				 float rotation = 0.0f,
				 glm::vec3 rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f),
				 float shininess = 0.0f,
				 bool shouldCullBackFaces = true);

private:
	static Mesh createMesh(unsigned int, unsigned int);
};

