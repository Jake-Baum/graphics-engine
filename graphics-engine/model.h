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
	Model(Mesh meshes...);

	void draw(Shader& shader);

private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> texturesLoaded;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	unsigned int textureFromFile(const char* path);
	std::optional<Texture> getLoadedTexture(const char* path);
};
#endif