#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct GLSLAttenuation {
	float constant;
	float linear;
	float quadratic;

	static float calculateRadius(const GLSLAttenuation& att, float lightMax) {
		float radius =
			(-att.linear + std::sqrtf(att.linear * att.linear - 4 * att.quadratic * (att.constant - (256.0f / (5.0f)) * lightMax)))
			/ (2 * att.quadratic);
		return radius;
	}
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

	static float calculateRadius(const GLSLPointLight& pointLight) {
		glm::vec3 lightColor = pointLight.color;
		float lightMax = std::fmaxf(std::fmaxf(lightColor.r, lightColor.g), lightColor.b);
		return GLSLAttenuation::calculateRadius(pointLight.attenuation, lightMax);
	}
};

struct GLSLSpotLight : public GLSLPointLight {
	glm::vec3 direction;
	float cutoffAngle; // radians

	static int calculateSizeAcross(const GLSLSpotLight& spotLight) {
		float attLength = GLSLPointLight::calculateRadius(spotLight);

		float size = tanf(spotLight.cutoffAngle) * attLength * 2;
		return size;
	}

	static glm::mat4 getLightSpaceMatrix(const GLSLSpotLight& spotLight) {
		glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
		if (glm::abs(spotLight.direction) == up)
			up = glm::vec3(0.f, 0.f, 1.f);

		float farPlane = GLSLPointLight::calculateRadius(spotLight);
		glm::mat4 viewMat = glm::lookAt(spotLight.position, spotLight.direction, up);
		glm::mat4 projectionMat = glm::perspective(spotLight.cutoffAngle, 1.0f, 0.1f, farPlane);

		return viewMat * projectionMat;
	}
};

