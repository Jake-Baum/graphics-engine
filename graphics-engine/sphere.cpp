#include "sphere.h"

Sphere::Sphere(Shader& shader,
							 unsigned int detailLevel,
							 glm::vec3 position,
							 glm::vec3 scale,
							 float rotation,
							 glm::vec3 rotationAxis,
							 float shininess,
							 bool shouldCullBackFaces) :
	Object(Model({createMesh(detailLevel, detailLevel)}), shader, position, scale, rotation, rotationAxis, shininess, shouldCullBackFaces)
{};

Mesh Sphere::createMesh(unsigned int stacksNumber, unsigned int slicesNumber)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	vertices.push_back({glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2()});

	for (int i = 0; i < stacksNumber - 1; i++)
	{
		auto phi = M_PI * double(i + 1) / double(stacksNumber);

		for (int j = 0; j < slicesNumber; j++)
		{
			auto theta = 2.0 * M_PI * double(j) / double(slicesNumber);
			auto x = std::sin(phi) * std::cos(theta);
			auto y = std::cos(phi);
			auto z = std::sin(phi) * std::sin(theta);

			auto normX = -std::sin(theta);
			auto normY = std::cos(phi);
			auto normZ = -std::cos(theta);

			vertices.push_back({glm::vec3(x, y, z), glm::vec3(normX, normY, normZ), glm::vec2()});
		}
	}

	vertices.push_back({glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2()});

	for (int i = 0; i < slicesNumber; i++)
	{
		indices.push_back(0);
		indices.push_back((i + 2) <= slicesNumber ? (i + 2) : 1);
		indices.push_back(i + 1);
	}
	for (int i = 1; i < stacksNumber - 1; i++)
	{
		for (int j = 0; j < slicesNumber; j++)
		{
			unsigned int topLeft = stacksNumber * (i - 1) + 1 + j;
			unsigned int topRight = (topLeft + 1) <= (stacksNumber * (i - 1) + slicesNumber) ? topLeft + 1 : (stacksNumber * (i - 1)) + 1;
			unsigned int bottomLeft = topLeft + slicesNumber;
			unsigned int bottomRight = topRight + slicesNumber;

			indices.push_back(topLeft);
			indices.push_back(topRight);
			indices.push_back(bottomLeft);

			indices.push_back(topRight);
			indices.push_back(bottomRight);
			indices.push_back(bottomLeft);
		}
	}
	for (int i = 0; i < slicesNumber; i++)
	{
		unsigned int bottomVertex = 1 + (slicesNumber * (stacksNumber - 1));
		unsigned int topLeft = bottomVertex - slicesNumber + i;
		indices.push_back(topLeft);
		indices.push_back((topLeft + 1) < bottomVertex ? (topLeft + 1) : bottomVertex - slicesNumber);
		indices.push_back(bottomVertex);
	}


 	return Mesh(vertices, indices, {
			{
				Model::constantColorTexture(glm::vec3(255.0f), glm::vec2(64.0f)),
				Mesh::TEXTURE_SPECULAR,
				"white.png"
			}
							});
}