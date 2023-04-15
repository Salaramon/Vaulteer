#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include <string>
#include <array>
#include <utility>

#include "Debug/Debug.h"

#include "Scene/Object3D.h"



struct PropertiesCamera {
public:
	float aspectRatio;
	float fov;
	float near;
	float far;
};

struct Frustum {
	explicit Frustum(std::array<glm::vec4, 6> arr) :
		left(arr[0]),
		right(arr[1]),
		bottom(arr[2]),
		top(arr[3]),
		near(arr[4]),
		far(arr[5])
	{}
	glm::vec4 left;
	glm::vec4 right;
	glm::vec4 bottom;
	glm::vec4 top;
	glm::vec4 near;
	glm::vec4 far;
};


class Camera : public Object3D {
public:

	Camera() :
		propertiesCamera(this->add<PropertiesCamera>(PropertiesCamera{
			.aspectRatio = 0.f,
			.fov = 45.f,
			.near = 1000.f,
			.far = 0.1f }))
	{}

	~Camera() {
		this->remove<PropertiesCamera>();
	}

	PropertiesCamera& propertiesCamera;
};



template<class... TupleArgs>
class CameraUtility : public Object3DUtility<TupleArgs...>, public Entity::Restricter<TupleArgs...> {
public:

	CameraUtility(const Camera& camera) : Object3DUtility<TupleArgs...>(camera),
		propertiesCamera(&camera.propertiesCamera) {}

	CameraUtility(TupleArgs... args) : Object3DUtility<TupleArgs...>(args...),
		propertiesCamera(Entity::tryGet<PropertiesCamera>(std::tie(args...)))
	{}

	CameraUtility(std::tuple<TupleArgs...> tuple) : Object3DUtility<TupleArgs...>(tuple),
		propertiesCamera(Entity::tryGet<PropertiesCamera>(tuple))
	{}


	template<class T = glm::mat4>
	CameraUtility::EnableWith<T, PropertiesCamera> projectionMatrix() const {
		glm::mat4 result = glm::perspective(glm::radians(propertiesCamera->fov), propertiesCamera->aspectRatio, propertiesCamera->near, propertiesCamera->far);

		return result;
	};

	template<class T = Frustum>
	CameraUtility::EnableWith<T, PropertiesCamera> frustum() const {
		std::array<glm::vec4, 6> frustumArray{};
		glm::mat4 projectionMatrix = projectionMatrix();

		std::array<std::function<size_t(const size_t&, const size_t&)>, 2> op = { std::plus<size_t>(), std::minus<size_t>() };
		for (size_t i = 0; i < frustumArray.size(); i++) {
			for (size_t j = 0; j < 4; j++) {
				frustumArray[i][j] = op[i % 2](projectionMatrix[j][3], projectionMatrix[j][i % 3]);
			}
		}

		Frustum frustum = Frustum(frustumArray);

		return frustum;
	};


	const PropertiesCamera* const propertiesCamera;
};

using CameraUtilityAll = CameraUtility<PropertiesCamera, Position3D, Rotation3D, Properties3D>;