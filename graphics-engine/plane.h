#pragma once
#include "object.h"

class Plane : public Object
{

public:
	Plane(Shader& shader,
				glm::vec3 position = DEFAULT_POSITION,
				glm::vec3 scale = DEFAULT_SCALE,
				float rotation = DEFAULT_ROTATION,
				glm::vec3 rotationAxis = DEFAULT_ROTATION_AXIS,
				float shininess = DEFAULT_SHININESS,
				bool shouldCullBackFaces = DEFAULT_SHOULD_CULL_BACK_FACES);

private:
	static const std::vector<Vertex> VERTICES;

	static const std::vector<unsigned int> INDICES;

};

