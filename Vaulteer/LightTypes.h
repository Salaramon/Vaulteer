#pragma once

#include <glm/glm.hpp>

struct GLSLBaseLight {
	glm::vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

struct GLSLDirectionalLight : public GLSLBaseLight {
	glm::vec3 direction;
};

struct GLSLPointLight : public GLSLBaseLight {
	glm::vec3 position;

	struct GLSLAttenuation {
		float constant;
		float linear;
		float quadratic;
	} attenuation;
};

struct GLSLSpotLight : public GLSLPointLight {
	glm::vec3 direction;
	float cutoff;
};

