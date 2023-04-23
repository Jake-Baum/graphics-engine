#include "cube.h"

const std::vector<Vertex> Cube::VERTICES = {
	{glm::vec3(-0.5f, -0.5f, 0.5f), glm::normalize(glm::vec3(-1.0f, -1.0f, 1.0f)), glm::vec2(0.0f, 0.0f)},
	{glm::vec3(0.5f, -0.5f, 0.5f), glm::normalize(glm::vec3(1.0f, -1.0f, 1.0f)), glm::vec2(1.0f, 0.0f)},
	{glm::vec3(0.5f, 0.5f, 0.5f), glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)), glm::vec2(1.0f, 1.0f)},
	{glm::vec3(-0.5f, 0.5f, 0.5f), glm::normalize(glm::vec3(-1.0f, 1.0f, 1.0f)), glm::vec2(0.0f, 1.0f)},
	{glm::vec3(-0.5f, -0.5f, -0.5f), glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)), glm::vec2(0.0f, 0.0f)},
	{glm::vec3(0.5f, -0.5f, -0.5f), glm::normalize(glm::vec3(1.0f, -1.0f, -1.0f)), glm::vec2(1.0f, 0.0f)},
	{glm::vec3(0.5f, 0.5f, -0.5f), glm::normalize(glm::vec3(1.0f, 1.0f, -1.0f)), glm::vec2(1.0f, 1.0f)},
	{glm::vec3(-0.5f, 0.5f, -0.5f), glm::normalize(glm::vec3(-1.0f, 1.0f, -1.0f)), glm::vec2(0.0f, 1.0f)}
};

const std::vector<unsigned int> Cube::INDICES = {0, 1, 2, 0, 2, 3, 4, 5, 1, 4, 1, 0, 7, 6, 5, 7, 5, 4, 3, 2, 6, 3, 6, 7, 4, 0, 3, 4, 3, 7, 1, 5, 6, 1, 6, 2};

Cube::Cube(Shader& shader, glm::vec3 position, glm::vec3 scale, float rotation, glm::vec3 rotationAxis, float shininess, bool shouldCullBackFaces) :
	Object(Model(
		{
			Mesh(
				std::vector<Vertex>({VERTICES}),
				std::vector<unsigned int>({INDICES}),
				std::vector<Texture>({
					{
						Model::constantColorTexture(glm::vec3(128.0f), glm::vec2(64.0f)),
						Mesh::TEXTURE_SPECULAR,
						"grey.png"
					}
				})
			)
		}), shader, position, scale, rotation, rotationAxis, shininess, shouldCullBackFaces)
{

}