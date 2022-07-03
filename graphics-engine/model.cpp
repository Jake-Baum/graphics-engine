#include "model.h"

Model::Model(const char* path)
{
	loadModel(path);
}

Model::Model(std::vector<Mesh> meshes)
{
	this->meshes = meshes;
}

Model::Model(Mesh meshes ...) : Model(std::vector<Mesh>({meshes}))
{}

void Model::draw(Shader& shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].draw(shader);
	}
}

void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);


	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "An error has occurred when loading model: " << importer.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	printf("Processing node %s, meshes: %u, children: %u\n", node->mName.C_Str(), node->mNumMeshes, node->mNumChildren);
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

		if (mesh->HasNormals())
		{
			vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		}

		if (mesh->mTextureCoords[0])
		{
			vertex.textureCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		else
		{
			vertex.textureCoords = glm::vec2(0.0f);
		}
		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, Mesh::TEXTURE_DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, Mesh::TEXTURE_SPECULAR);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		std::optional<Texture> loadedTexture = getLoadedTexture(str.C_Str());

		if (loadedTexture.has_value())
		{
			textures.push_back(loadedTexture.value());
		}
		else
		{
			Texture texture = {
				textureFromFile(str.C_Str()),
				typeName,
				str.C_Str()
			};

			textures.push_back(texture);
			texturesLoaded.push_back(texture);
		}
	}

	return textures;
}

unsigned int Model::constantColorTexture(glm::vec4 color, glm::vec2 dimensions)
{
	std::vector<unsigned char> data;

	for (int i = 0; i < dimensions.x * dimensions.y; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			data.push_back(color[j]);
		}
	}

	return generateAndBindTexture(&data[0], GL_RGBA, dimensions.x, dimensions.y);
}


unsigned int Model::textureFromFile(const char* path)
{
	stbi_set_flip_vertically_on_load(true);

	int width, height, numberOfChannels;
	unsigned char* data = stbi_load(path, &width, &height, &numberOfChannels, 0);
	if (!data)
	{
		std::cout << "Failed to load texture " << path << ": " << stbi_failure_reason() << std::endl;
		throw - 1;
	}

	GLenum format;
	switch (numberOfChannels)
	{
	case 1:
		format = GL_RED;
		break;
	case 3:
		format = GL_RGB;
		break;
	case 4:
		format = GL_RGBA;
		break;
	default:
		throw - 1;
	}

	unsigned int texture = generateAndBindTexture(data, format, width, height);

	stbi_image_free(data);

	return texture;
}

unsigned int Model::generateAndBindTexture(unsigned char* data, GLenum format, unsigned int width, unsigned int height)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	return texture;
}

std::optional<Texture> Model::getLoadedTexture(const char* path)
{
	for (unsigned int i = 0; i < texturesLoaded.size(); i++)
	{
		if (std::strcmp(texturesLoaded[i].path.data(), path) == 0)
		{
			return std::optional(texturesLoaded[i]);
		}
	}

	std::optional<Texture> empty;
	return empty;
}
