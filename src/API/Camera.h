#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include <string>
#include <array>
#include <utility>

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
	PropertiesCamera& propertiesCamera;

	Camera() :
		propertiesCamera(this->add<PropertiesCamera>(PropertiesCamera{
			.aspectRatio = 1.0f,
			.fov = 45.f,
			.near = 0.1f,
			.far = 1000.f }))
	{}

	~Camera() {
		this->remove<PropertiesCamera>();
	}


	// calculation methods

	static glm::mat4 projectionMatrix(const PropertiesCamera& propertiesCamera) {
		return glm::perspective(glm::radians(propertiesCamera.fov), propertiesCamera.aspectRatio, propertiesCamera.near, propertiesCamera.far);
	}

	static Frustum frustum(const PropertiesCamera& propertiesCamera) {
		std::array<glm::vec4, 6> frustumArray{};
		glm::mat4 projection = projectionMatrix(propertiesCamera);

		std::array<std::function<size_t(const size_t&, const size_t&)>, 2> op = { std::plus<size_t>(), std::minus<size_t>() };
		for (size_t i = 0; i < frustumArray.size(); i++) {
			for (size_t j = 0; j < 4; j++) {
				frustumArray[i][j] = op[i % 2](projection[j][3], projection[j][i % 3]);
			}
		}

		return Frustum(frustumArray);
	}

};


using CameraTuple = std::tuple<PropertiesCamera, Position3D, Rotation3D, Properties3D>;

class CameraReference {
public:
	const PropertiesCamera* propertiesCamera;
	const Position3D* position;
	const Rotation3D* rotation;
	const Properties3D* properties3D;

	CameraReference(const Camera& camera) :
		propertiesCamera(&camera.propertiesCamera), position(camera.position), rotation(camera.rotation), properties3D(camera.properties3D) {}

	CameraReference(const CameraTuple& tuple) :
		propertiesCamera(&std::get<0>(tuple)), position(&std::get<1>(tuple)), rotation(&std::get<2>(tuple)), properties3D(&std::get<3>(tuple)) {}

	CameraReference(const CameraReference& other) = default;

	glm::mat4 viewMatrix() const {
		return Object3D::viewMatrix(*position, *rotation);
	}

	glm::mat4 projectionMatrix() const {
		return Camera::projectionMatrix(*propertiesCamera);
	}

	Frustum frustum() const {
		return Camera::frustum(*propertiesCamera);
	}
};

