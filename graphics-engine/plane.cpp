#include "plane.h"

const std::vector<Vertex> Plane::VERTICES = {
		{glm::vec3(-0.5f, 0.0f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
		{glm::vec3(0.5f, 0.0f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
		{glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
		{glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)}
};

const std::vector<unsigned int> Plane::INDICES = {0, 1, 2, 0, 2, 3};

Plane::Plane(Shader& shader, glm::vec3 position, glm::vec3 scale, float rotation, glm::vec3 rotationAxis, float shininess, bool shouldCullBackFaces): 
	Object(Model({
		Mesh(
			std::vector<Vertex>({VERTICES}),
			std::vector<unsigned int>({INDICES}),
			std::vector<Texture>({
				{
					Model::constantColorTexture(glm::vec3(128.0f), glm::vec2(64.0f)),
					Mesh::TEXTURE_SPECULAR,
					"grey.png"
				},
				{
					Model::constantColorTexture(glm::vec4(0.0f), glm::vec2(64.0f)),
					Mesh::TEXTURE_SPECULAR,
					"black.png"
				}
			})
		)
	}), shader, position, scale, rotation, rotationAxis, shininess, shouldCullBackFaces)
{
	
}
