#include "vpch.h"
#include "Scene/Object3D.h"

Object3D::Object3D() :
	modelScale(glm::mat4(1.0f)),
	modelTranslation(glm::mat4(1.0f)),
	modelRotation(glm::mat4(1.0f)),
	OR(this,
		DY::V(&boundingSphere, &modelTranslation, &modelScale, &modelRotation),
		DY::N("boundingSphere", "modelTranslation", "modelScale", "modelRotation")) {}

Object3D::Object3D(Object3D&& object3D) :
	modelScale(object3D.modelScale),
	modelTranslation(object3D.modelTranslation),
	modelRotation(object3D.modelRotation),
	OR(this,
	   DY::V(&boundingSphere, &modelTranslation, &modelScale, &modelRotation),
	   DY::N("boundingSphere", "modelTranslation", "modelScale", "modelRotation"))
{}

glm::mat4 Object3D::getModelMatrix() const {
	return modelTranslation * modelScale * modelRotation;
}

void Object3D::rotate(float angle, glm::vec3 axis) {
	LOG::CLAS::debug<&Object3D::rotate>(this, &modelRotation, DY::std_format("model to be rotated by angle {} around axis {}", DY::std_to_string(angle), DY::glm_to_string(axis)));
	modelRotation = glm::rotate(modelRotation, angle, axis);
}

void Object3D::setRotation(float angle, glm::vec3 axis) {
	LOG::CLAS::debug<&Object3D::setRotation>(this, &modelRotation, DY::std_format("model rotation to be set to angle {} around axis {}", DY::std_to_string(angle), DY::glm_to_string(axis)));
	modelRotation = glm::rotate(glm::mat4(1.0f), angle, axis);
}

void Object3D::move(float x, float y, float z) {
	glm::vec3 position = glm::vec3(x, y, z);
	LOG::CLAS::debug<DY::OverloadSelector<Object3D, void(float, float, float)>::Get<&Object3D::move>>(this, &modelTranslation, DY::std_format("model to be tranlated by: {}", DY::glm_to_string(position)));
	modelTranslation = glm::translate(modelTranslation, position);
}

void Object3D::move(glm::vec3 position) {
	LOG::CLAS::debug<DY::OverloadSelector<Object3D, void(glm::vec3)>::Get<&Object3D::move>>(this, &modelTranslation, DY::std_format("model to be tranlated by: {}", DY::glm_to_string(position)));
	modelTranslation = glm::translate(modelTranslation, position);
}

void Object3D::setPosition(float x, float y, float z) {
	glm::vec3 position = glm::vec3(x, y, z);
	LOG::CLAS::debug<DY::OverloadSelector<Object3D, void(float, float, float)>::Get<&Object3D::setPosition>>(this, &modelTranslation, DY::std_format("model translation to be set to: {}", DY::glm_to_string(position)));
	modelTranslation = glm::translate(glm::mat4(1.0f), position);
}

void Object3D::setPosition(glm::vec3 position) {
	LOG::CLAS::debug<DY::OverloadSelector<Object3D, void(glm::vec3)>::Get<&Object3D::setPosition>>(this, &modelTranslation, DY::std_format("model translation to be set to: {}", DY::glm_to_string(position)));
	modelTranslation = glm::translate(glm::mat4(1.0f), position);
}

void Object3D::scale(float x, float y, float z) {
	glm::vec3 scale = glm::vec3(x, y, z);
	LOG::CLAS::debug<DY::OverloadSelector<Object3D, void(float, float, float)>::Get<&Object3D::scale>>(this, &modelScale, DY::std_format("model to be scaled by: {}", DY::glm_to_string(scale)));
	modelScale = glm::scale(modelScale, scale);
}

void Object3D::scale(glm::vec3 scale) {
	LOG::CLAS::debug<DY::OverloadSelector<Object3D, void(glm::vec3)>::Get<&Object3D::scale>>(this, &modelScale, DY::std_format("model to be scaled by: {}", DY::glm_to_string(scale)));
	modelScale = glm::scale(modelScale, scale);
}

void Object3D::setScale(float x, float y, float z) {
	glm::vec3 scale = glm::vec3(x, y, z);
	LOG::CLAS::debug<DY::OverloadSelector<Object3D, void(float, float, float)>::Get<&Object3D::setScale>>(this, &modelScale, DY::std_format("model scale to be set to: {}", DY::glm_to_string(scale)));
	modelScale = glm::scale(glm::mat4(1.0f), scale);
}

void Object3D::setScale(glm::vec3 scale) {
	LOG::CLAS::debug<DY::OverloadSelector<Object3D, void(glm::vec3)>::Get<&Object3D::setScale>>(this, &modelScale, DY::std_format("model scale to be set to: {}", DY::glm_to_string(scale)));
	modelScale = glm::scale(glm::mat4(1.0f), scale);
}

glm::vec3 Object3D::getPosition() {
	return glm::vec3(
		modelTranslation[3][0],
		modelTranslation[3][1],
		modelTranslation[3][2]
	);
}
