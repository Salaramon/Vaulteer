#pragma once

#include <corecrt_math_defines.h>
#include <type_traits>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace std {

	template<class Type> requires (std::is_floating_point_v<Type>)
		constexpr Type ctradians(Type deg) {
		return (M_PI / 180) * deg;
	}

	template<class Type> requires (std::is_floating_point_v<Type>)
		constexpr Type ctdegrees(Type rad) {
		return (180 / M_PI) * rad;
	}

}


static std::vector<glm::vec4> getFrustumCorners(glm::mat4 projectionMatrix) {

	// extract parameters from projection
	float aspect_ratio_h = projectionMatrix[1][1] / projectionMatrix[0][0];
	float zNear = projectionMatrix[3][2] / (projectionMatrix[2][2] - 1.0);
	float zFar = projectionMatrix[3][2] / (projectionMatrix[2][2] + 1.0);
	float tanHalfFOV = 1.0f / projectionMatrix[1][1];

	float xn = zNear * tanHalfFOV * aspect_ratio_h;
	float yn = zNear * tanHalfFOV;
	float xf = zFar * tanHalfFOV * aspect_ratio_h;
	float yf = zFar * tanHalfFOV;

	std::vector<glm::vec4> frustumCorners(8);
	// near plane
	frustumCorners[0] = glm::vec4(-xn, -yn, -zNear, 1.0f);
	frustumCorners[1] = glm::vec4(xn, -yn, -zNear, 1.0f);
	frustumCorners[2] = glm::vec4(-xn, yn, -zNear, 1.0f);
	frustumCorners[3] = glm::vec4(xn, yn, -zNear, 1.0f);
	// far plane
	frustumCorners[4] = glm::vec4(-xf, -yf, -zFar, 1.0f);
	frustumCorners[5] = glm::vec4(xf, -yf, -zFar, 1.0f);
	frustumCorners[6] = glm::vec4(-xf, yf, -zFar, 1.0f);
	frustumCorners[7] = glm::vec4(xf, yf, -zFar, 1.0f);

	return frustumCorners;
}

static std::vector<glm::vec4> getFrustumCornersWorldSpace(glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {
	std::vector<glm::vec4> frustumCorners = getFrustumCorners(projectionMatrix);
	glm::mat4 viewInverseMat = glm::inverse(viewMatrix);

	for (int i = 0; i < frustumCorners.size(); i++) {
		glm::vec4 corner = viewInverseMat * frustumCorners[i];
		frustumCorners[i] = corner;
	}

	return frustumCorners;
}
