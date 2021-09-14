#include "Model.h"


Model::Model(ModelData& data) :
	model(&data),
	modelRotation(glm::mat4(1.0f)),
	modelScale(glm::mat4(1.0f)),
	modelTranslation(glm::mat4(1.0f))
{
}

glm::mat4 Model::getModelMatrix() {
	return modelTranslation * modelScale * modelRotation;
}

void Model::rotate(float angle, glm::vec3 axis)
{
	modelRotation = glm::rotate(modelRotation, angle, axis);
}

void Model::setRotation(float angle, glm::vec3 axis)
{
	modelRotation = glm::rotate(glm::mat4(1.0f), angle, axis);
}

void Model::move(float x, float y, float z)
{
	modelTranslation = glm::translate(modelTranslation, glm::vec3(x, y, z));
}

void Model::setPosition(float x, float y, float z)
{
	modelTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
}

void Model::scale(float x, float y, float z)
{
	modelScale = glm::scale(modelScale, glm::vec3(x, y, z));
}

void Model::setScale(float x, float y, float z)
{
	modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(x, y, z));
}

void Model::render(const Shader& shader)
{
	model->draw(shader);
}


