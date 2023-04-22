#pragma once
#include "object.h"

class Cube : public Object
{

public:
	Cube(Shader& shader,
			 glm::vec3 position = glm::vec3(0.0f),
			 glm::vec3 scale = glm::vec3(1.0f),
			 float rotation = 0.0f,
			 glm::vec3 rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f),
			 float shininess = 0.0f,
			 bool shouldCullBackFaces = true);

private:
	static const std::vector<Vertex> VERTICES;

	static const std::vector<unsigned int> INDICES;

};

