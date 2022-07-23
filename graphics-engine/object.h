#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>

#include "model.h"
#include "shader.h"
#include "point_light.h"

class Object
{
public:
	Object(Model model,
				 Shader& shader,
				 glm::vec3 position = glm::vec3(0.0f),
				 glm::vec3 scale = glm::vec3(1.0f),
				 float rotation = 0.0f,
				 glm::vec3 rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f),
				 float shininess = 64.0f);

	void draw();
	
	glm::vec3 getPosition();

	std::optional<Shader*> outlineShader;
	std::optional<PointLight> light;

private:
	Model model;
	Shader* shader;
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::vec3 position;
	glm::vec3 scale;
	float rotation;
	glm::vec3 rotationAxis;
	float shininess;
};
#endif