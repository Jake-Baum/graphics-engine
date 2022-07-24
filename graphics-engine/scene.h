#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <map>
#include <set>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "object.h"
#include "camera.h"

class Scene
{
public:
	Scene(Shader& lightingShader, Camera& camera);

	void draw(GLFWwindow* window);

	void addObjects(std::vector<Object> objects);

private:
	void configureLighting();
	Shader* lightingShader;
	Camera* camera;
	std::vector<Object> objects;
};

#endif