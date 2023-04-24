#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>

#include "model.h"
#include "shader.h"
#include "point_light.h"
#include "directional_light.h"

class Object
{
public:
	Object(Model model,
				 Shader& shader,
				 glm::vec3 position = DEFAULT_POSITION,
				 glm::vec3 scale = DEFAULT_SCALE,
				 float rotation = DEFAULT_ROTATION,
				 glm::vec3 rotationAxis = DEFAULT_ROTATION_AXIS,
				 float shininess = DEFAULT_SHININESS,
				 bool shouldCullBackFaces = DEFAULT_SHOULD_CULL_BACK_FACES);

	Object(glm::vec3 position = DEFAULT_POSITION,
				 glm::vec3 scale = DEFAULT_SCALE,
				 float rotation = DEFAULT_ROTATION,
				 glm::vec3 rotationAxis = DEFAULT_ROTATION_AXIS,
				 float shininess = DEFAULT_SHININESS,
				 bool shouldCullBackFaces = DEFAULT_SHOULD_CULL_BACK_FACES);

	void draw();

	glm::vec3 getPosition();
	Shader* getShader();

	static const glm::vec3 DEFAULT_POSITION;
	static const glm::vec3 DEFAULT_SCALE;
	static const float DEFAULT_ROTATION;
	static const glm::vec3 DEFAULT_ROTATION_AXIS;
	static const float DEFAULT_SHININESS;
	static const float DEFAULT_SHOULD_CULL_BACK_FACES;

	std::optional<Shader*> outlineShader;
	std::vector<DirectionalLight> directionalLights;
	std::vector<PointLight> pointLights;

private:
	std::optional<Model> model;
	Shader* shader;
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::vec3 position;
	glm::vec3 scale;
	float rotation;
	glm::vec3 rotationAxis;
	float shininess;
	bool shouldCullBackFaces;
};
#endif