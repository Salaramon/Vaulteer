#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Debug/Debug.h"

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

public:

	inline static auto CR = DY::ClassRegister<
		&getModelMatrix,
		&rotate,
		&setRotation,
		DY::OverloadSelector<Object3D, void(float, float, float)>::Get<&move>,
		DY::OverloadSelector<Object3D, void(glm::vec3)>::Get<&move>,
		DY::OverloadSelector<Object3D, void(float, float, float)>::Get<&setPosition>,
		DY::OverloadSelector<Object3D, void(glm::vec3)>::Get<&setPosition>,
		DY::OverloadSelector<Object3D, void(float, float, float)>::Get<&scale>,
		DY::OverloadSelector<Object3D, void(glm::vec3)>::Get<&scale>,
		DY::OverloadSelector<Object3D, void(float, float, float)>::Get<&setScale>,
		DY::OverloadSelector<Object3D, void(glm::vec3)>::Get<&setScale>,
		&getPosition>(
			"getModelMatrix",
			"rotate",
			"setRotation",
			"move(...3)",
			"move(...1)",
			"setPosition(...3)",
			"setPosition(...1",
			"scale(...3)",
			"scale(...1",
			"setScale(...3)",
			"setScale(...1)",
			"getPosition");
	inline static auto CB = DY::ClassBinder<decltype(CR)>();

	DY::ObjectRegister<Object3D,
		decltype(boundingSphere),
		decltype(modelTranslation),
		decltype(modelScale),
		decltype(modelRotation)> OR;
	inline static auto OB = DY::ObjectBinder<decltype(OR)>();

	using LOG = _LOG<decltype(CB), decltype(OB), DY::No_FB, DY::No_VB>;

};

