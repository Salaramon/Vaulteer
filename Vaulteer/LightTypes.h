#pragma once

#include <glm/glm.hpp>

struct GLSLAttenuation {
	float constant;
	float linear;
	float quadratic;
};

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
	GLSLAttenuation attenuation;
	float radius;

	static float calculateRadius(GLSLPointLight pointLight) {
		GLSLAttenuation att = pointLight.attenuation;
		glm::vec3 lightColor = pointLight.color;

		float lightMax = std::fmaxf(std::fmaxf(lightColor.r, lightColor.g), lightColor.b);
		float radius =
			(-att.linear + std::sqrtf(att.linear * att.linear - 4 * att.quadratic * (att.constant - (256.0f / (5.0f)) * lightMax)))
			/ (2 * att.quadratic);
		return radius;
	}
};

struct GLSLSpotLight : public GLSLBaseLight {
	glm::vec3 position;
	glm::vec3 direction;
	GLSLAttenuation attenuation;
	float cutoff;
};

