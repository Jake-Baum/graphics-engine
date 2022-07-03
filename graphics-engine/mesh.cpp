#include "mesh.h"

const char* Mesh::TEXTURE_DIFFUSE = "textureDiffuse";
const char* Mesh::TEXTURE_SPECULAR = "textureSpecular";

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setUpMesh();
}

void Mesh::draw(Shader& shader)
{
  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;
  for (unsigned int i = 0; i < textures.size(); i++)
  {
    glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
    // retrieve texture number (the N in diffuse_textureN)
    std::string number;
    std::string name = textures[i].type;
    if (name == TEXTURE_DIFFUSE)
    {
      number = std::to_string(diffuseNr++);
    }
    else if (name == TEXTURE_SPECULAR)
    {
      number = std::to_string(specularNr++);
    }

    shader.setInt(("material." + name + number).c_str(), i);
    glBindTexture(GL_TEXTURE_2D, textures[i].id);
  }
  glActiveTexture(GL_TEXTURE0);

  // draw mesh
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void Mesh::setUpMesh()
{
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               &indices[0], GL_STATIC_DRAW);

  // vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  // vertex normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
  // vertex texture coords
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoords));

  glBindVertexArray(0);
}
