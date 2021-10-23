#include "Model.h"


Model::Model(Model&& model) :
	model(std::move(model.model)),
	modelScale(std::move(model.modelScale)),
	modelTranslation(std::move(model.modelTranslation)),
	modelRotation(std::move(model.modelRotation)),
	polygonFaces(std::move(model.polygonFaces)),
	polygonMode(std::move(model.polygonMode)),
	polygonLineWidth(std::move(model.polygonLineWidth))
{}

Model::Model(GraphicsData& data) :
	model(&data),
	modelRotation(glm::mat4(1.0f)),
	modelScale(glm::mat4(1.0f)),
	modelTranslation(glm::mat4(1.0f))
{
}

Model::Model(ModelData& data) :
	model(&data),
	modelRotation(glm::mat4(1.0f)),
	modelScale(glm::mat4(1.0f)),
	modelTranslation(glm::mat4(1.0f))
{
}

Model::Model(LineData& data) :
	model(&data),
	modelRotation(glm::mat4(1.0f)),
	modelScale(glm::mat4(1.0f)),
	modelTranslation(glm::mat4(1.0f))
{
	setPolygonMode(Model::Polygon::Line);
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

void Model::setPolygonFaces(GLenum faces)
{
	polygonFaces = faces;
}

void Model::setPolygonMode(GLenum mode)
{
	polygonMode = mode;
}

void Model::setPolygonLineWidth(GLfloat width)
{
	polygonLineWidth = width;
}

GLenum Model::getPolygonFaces()
{
	return polygonFaces;
}

GLenum Model::getPolygonMode()
{
	return polygonMode;
}

GLfloat Model::getPolygonLineWidth()
{
	return polygonLineWidth;
}

GraphicsData* Model::getData()
{
	return model;
}

void Model::render(const Shader& shader)
{
	model->draw(shader);
}


