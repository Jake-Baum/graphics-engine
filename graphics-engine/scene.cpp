#include "scene.h"

Scene::Scene(Shader& lightingShader, Camera& camera)
{
	this->lightingShader = &lightingShader;
	this->camera = &camera;
}

void Scene::draw(GLFWwindow* window)
{
	configureLighting();

	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glm::mat4 projection = glm::perspective(glm::radians(camera->zoom), (float)width / height, 0.1f, 100.0f);

	std::multimap<float, Object> sorted;
	std::set<Shader*> shaders;
	for (auto& object : objects)
	{
		float distance = glm::length(camera->position - object.getPosition());
		sorted.insert(std::make_pair(distance, object));

		if (shaders.find(object.getShader()) == shaders.end())
		{
			shaders.insert(object.getShader());
			object.getShader()->use();
			object.getShader()->setMat4("view", camera->getViewMatrix());
			object.getShader()->setMat4("projection", projection);
		}
	}
	for (std::map<float, Object>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
	{
		it->second.draw();
	}
}

void Scene::addObjects(std::vector<Object> objects)
{
	this->objects.insert(this->objects.end(), objects.begin(), objects.end());
}

void Scene::configureLighting()
{
	lightingShader->use();
	lightingShader->resetLights();
	lightingShader->setVec3("viewPosition", camera->position);

	for (auto& object : objects)
	{
		for (auto& light : object.directionalLights)
		{
			lightingShader->addDirectionalLight(light);
		}
		for (auto& light : object.pointLights)
		{
			lightingShader->addPointLight(light);
		}
	}
}
