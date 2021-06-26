#pragma once

#include <glm/glm.hpp>

struct BaseLight {
	glm::vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight : public BaseLight {
	glm::vec3 direction;
};

struct PointLight : public BaseLight {
	glm::vec3 position;

	struct Attenuation {
		float constant;
		float linear;
		float quadratic;
	} attenuation;
};

struct SpotLight : public PointLight {
	glm::vec3 direction;
	float cutoff;
};

