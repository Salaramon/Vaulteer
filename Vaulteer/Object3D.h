#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Object3D
{
public:
	Object3D();
	Object3D(Object3D&& model);

	glm::mat4 getModelMatrix();
	void rotate(float angle, glm::vec3 axis);
	void setRotation(float angle, glm::vec3 axis);
	void move(float x, float y, float z);
	void move(glm::vec3 position);
	void setPosition(float x, float y, float z);
	void setPosition(glm::vec3 position);
	void scale(float x, float y, float z);
	void scale(glm::vec3 scale);
	void setScale(float x, float y, float z);
    void setScale(glm::vec3 scale);

	glm::vec3 getPosition();

private:
	glm::vec4 boundingSphere;

	glm::mat4 modelScale;
	glm::mat4 modelTranslation;
	glm::mat4 modelRotation;
};

