#pragma once

#include <glm/glm.hpp>

struct BaseLight
{
	glm::vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

struct PointLight : public BaseLight
{
	glm::vec3 position;

	struct Attenuation {
		float constant;
		float linear;
		float exp;
	} attenuation;
};

struct DirectionalLight : public BaseLight
{
	glm::vec3 direction;
};

