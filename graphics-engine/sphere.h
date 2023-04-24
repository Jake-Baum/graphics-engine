#pragma once
#include "object.h"

#define _USE_MATH_DEFINES
#include <math.h>

// Sphere of radius 1
class Sphere : public Object
{
public:
	Sphere(Shader& shader,
				 unsigned int detailLevel,
				 glm::vec3 position = DEFAULT_POSITION,
				 glm::vec3 scale = DEFAULT_SCALE,
				 float rotation = DEFAULT_ROTATION,
				 glm::vec3 rotationAxis = DEFAULT_ROTATION_AXIS,
				 float shininess = DEFAULT_SHININESS,
				 bool shouldCullBackFaces = DEFAULT_SHOULD_CULL_BACK_FACES);

private:
	static Mesh createMesh(unsigned int, unsigned int);
};

