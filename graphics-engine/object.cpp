#include "object.h"

Object::Object(Model model, Shader& shader, glm::vec3 position, glm::vec3 scale, float rotation, glm::vec3 rotationAxis, float shininess):
	model(model), position(position), scale(scale), rotation(rotation), rotationAxis(rotationAxis), shininess(shininess)
{
	this->shader = &shader;
	this->modelMatrix = glm::translate(this->modelMatrix, position);
	this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(rotation), rotationAxis);
	this->modelMatrix = glm::scale(this->modelMatrix, scale);
}

void Object::draw()
{
	if (outlineShader.has_value())
	{
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
	}

	shader->use();
	shader->setMat4("model", modelMatrix);
	shader->setFloat("material.shininess", shininess);
	model.draw(*shader);

	if (outlineShader.has_value())
	{
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		outlineShader.value()->use();
		outlineShader.value()->setVec4("color", glm::vec4(0.75f, 0.75f, 0.2f, 1.0f));

		glm::mat4 outlineModel = glm::scale(modelMatrix, glm::vec3(1.01f));
		outlineShader.value()->setMat4("model", outlineModel);
		model.draw(*outlineShader.value());

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);

		glClear(GL_STENCIL_BUFFER_BIT);
		glStencilMask(0x00);
	}

	if (light.has_value())
	{
		light.value().setLightUniforms();
	}
}

glm::vec3 Object::getPosition()
{
	return position;
}
