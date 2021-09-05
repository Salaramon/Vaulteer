#pragma once

#include <glad/glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>

#include <vector>

#include "MyCamera.h"
#include "MathUtils.h"

class ShadowCascade
{
public:
	ShadowCascade(float zNear, float zFar);

	glm::mat4 getLightSpaceMatrix();

	void setView(const glm::mat4& view);
	void setProjection(const glm::mat4& projection);
	void updateBounds(MyCamera& camera, glm::vec3 lightDirection);

	std::vector<float> bounds;
	std::vector<glm::vec4> cameraFrustumWS;

private:
	float zNear, zFar;
	glm::mat4 lightView, lightProjection;

	glm::vec4 frustumCorners[8];
};

