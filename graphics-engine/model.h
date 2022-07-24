#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include <optional>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "shader.h"
#include "mesh.h"
#include "stb_image.h"

class Model 
{
public:

	Model(const char* path);
	Model(std::vector<Mesh> meshes);

	static unsigned int textureFromFile(const char* path);
	static unsigned int constantColorTexture(glm::vec3 color, glm::vec2 dimensions);
	static unsigned int constantColorTexture(glm::vec4 color, glm::vec2 dimensions);
	void draw(Shader& shader);

private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> texturesLoaded;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	static unsigned int generateAndBindTexture(unsigned char* data, GLenum format, unsigned int width, unsigned int height);
	std::optional<Texture> getLoadedTexture(const char* path);
};
#endif