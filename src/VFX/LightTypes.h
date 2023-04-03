#pragma once

#include <corecrt_math_defines.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Attenuation {
	float constant;
	float linear;
	float quadratic;

	float calculateRadius(float lightMax) const {
		float radius =
			(-linear + std::sqrtf(linear * linear - 4 * quadratic * (constant - (256.0f / (5.0f)) * lightMax)))
			/ (2 * quadratic);
		return radius;
	}
};

struct BaseLight {
	glm::vec3 color;
	float ambientIntensity;
	float diffuseIntensity;

	float getLightMax() const {
		return std::fmaxf(std::fmaxf(color.r, color.g), color.b);
	}
};

struct DirectionalLight {
	BaseLight light;
	glm::vec3 direction;
};

struct PointLight {
	// radius of light volume rendering sphere asset
	inline static float volumeSphereRadius = 0.45f;

	BaseLight light;
	Attenuation attenuation;
	glm::vec3 position;
	float radius;
	//glm::mat4 instanceMat;

	PointLight(Attenuation a, BaseLight l, glm::vec3 position) : light(l), attenuation(a), position(position) {
		radius = calculatePointRadius();
		//instanceMat = glm::translate(glm::scale(glm::mat4(1.0), glm::vec3(radius / volumeSphereRadius)), position);
	}

	float calculatePointRadius() const {
		return attenuation.calculateRadius(light.getLightMax());
	}
};

struct alignas(16) SpotLight {
	BaseLight light;
	Attenuation attenuation;
	glm::vec3 position;
	float radius;
	glm::vec3 direction;
	float cutoffAngle; // radians
	glm::mat4 lightSpaceMatrix;

	SpotLight(Attenuation a, BaseLight l, glm::vec3 position, glm::vec3 direction, float angle)
			: light(l), attenuation(a), position(position), direction(direction), cutoffAngle(angle) {
		radius = calculatePointRadius();
		lightSpaceMatrix = getLightSpaceMatrix();
	}

	
	float calculatePointRadius() const {
		return attenuation.calculateRadius(light.getLightMax());
	}

	float calculateSizeAcross() const {
		float attLength = attenuation.calculateRadius(light.getLightMax());

		float size = tanf(cutoffAngle) * attLength * 2;
		return size;
	}

	// matrix calculation methods

	glm::mat4 getProjectionMatrix() const {
		float farPlane = calculatePointRadius() * 8; // TODO fix the math here
		return glm::perspective(cutoffAngle, 1.0f, 4.0f, farPlane);
	}

	glm::mat4 getViewMatrix() const {
		glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
		if (glm::abs(direction) == up)
			up = glm::vec3(0.f, 0.f, 1.f);

		return glm::lookAt(position, position + direction, up);
	}

	glm::mat4 getLightSpaceMatrix() const {
		return getProjectionMatrix() * getViewMatrix();
	}
};

