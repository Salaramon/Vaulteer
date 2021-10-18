#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <iostream>

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

	std::vector<glm::vec4> cameraFrustumWS;
	glm::vec4 cascadeCorners[8];

	float zNear, zFar;
private:
	glm::mat4 lightView, lightProjection;

	glm::vec4 frustumCorners[8];
};

