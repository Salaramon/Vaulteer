#include "Object3D.h"

Object3D::Object3D() : 
	modelRotation(glm::mat4(1.0f)),
	modelScale(glm::mat4(1.0f)),
	modelTranslation(glm::mat4(1.0f))
{}

Object3D::Object3D(Object3D&& object3D) :
	modelScale(std::move(object3D.modelScale)),
	modelTranslation(std::move(object3D.modelTranslation)),
	modelRotation(std::move(object3D.modelRotation))
{}

glm::mat4 Object3D::getModelMatrix() {
	return modelTranslation * modelScale * modelRotation;
}

void Object3D::rotate(float angle, glm::vec3 axis)
{
	modelRotation = glm::rotate(modelRotation, angle, axis);
}

void Object3D::setRotation(float angle, glm::vec3 axis)
{
	modelRotation = glm::rotate(glm::mat4(1.0f), angle, axis);
}

void Object3D::move(float x, float y, float z)
{
	modelTranslation = glm::translate(modelTranslation, glm::vec3(x, y, z));
}

void Object3D::setPosition(float x, float y, float z)
{
	modelTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
}

void Object3D::scale(float x, float y, float z)
{
	modelScale = glm::scale(modelScale, glm::vec3(x, y, z));
}

void Object3D::setScale(float x, float y, float z)
{
	modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(x, y, z));
}