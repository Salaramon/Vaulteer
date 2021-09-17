#include "ShadowCascade.h"

ShadowCascade::ShadowCascade(float zNear, float zFar) :
	zNear(zNear - 1.0f), zFar(zFar + 1.0f),
	lightView(glm::mat4()), lightProjection(glm::mat4()) {
	for (int i = 0; i < 8; i++) {
		glm::vec4 corner((float)((i & 4) / 2 - 1), (float)((i & 2) - 1), (float)((i & 1) * 2 - 1), 1.0f);
		frustumCorners[i] = corner;
	}
}

glm::mat4 ShadowCascade::getLightSpaceMatrix() {
	return lightProjection * lightView;
}

void ShadowCascade::setView(const glm::mat4& view) {
	lightView = view;
}

void ShadowCascade::setProjection(const glm::mat4& projection) {
	lightProjection = projection;
}

void ShadowCascade::updateBounds(MyCamera& camera, glm::vec3 lightDirection) {
	// calculate frustum corners

 	float fov = camera.camera_fov;
	float aspect_ratio_h = camera.width / camera.height;
	float tanHalfFOV = tanf(std::ctradians(fov / 2));

	float xn = zNear * tanHalfFOV * aspect_ratio_h;
	float yn = zNear * tanHalfFOV;
	float xf = zFar * tanHalfFOV * aspect_ratio_h;
	float yf = zFar * tanHalfFOV;

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

	// set light as origin in light view matrix
	glm::mat4 lightMat = glm::lookAt(glm::vec3(0.0f), lightDirection, glm::vec3(0.f, 1.f, 0.f));
	lightView = lightMat;

	// get inverse camera matrix, but flip camera orientation to not render shadows behind us
	glm::mat4 cameraInverseMat = glm::inverse(camera.getViewMatrix());

	// find minimum bounding box for frustum (in light space)

	glm::vec4 frustumCornersLightSpace[8];
	float minX = std::numeric_limits<float>::max();
	float maxX = -std::numeric_limits<float>::max();
	float minY = std::numeric_limits<float>::max();
	float maxY = -std::numeric_limits<float>::max();
	float minZ = std::numeric_limits<float>::max();
	float maxZ = -std::numeric_limits<float>::max();

	cameraFrustumWS.clear();
	for (unsigned int i = 0; i < 8; i++) {
		// Transform the frustum coordinate from camera to world space
		glm::vec4 vW = cameraInverseMat * frustumCorners[i];
		cameraFrustumWS.push_back(vW);

		// Transform the frustum coordinate from world to light space
		frustumCornersLightSpace[i] = lightMat * vW;

		minX = fmin(minX, frustumCornersLightSpace[i].x);
		maxX = fmax(maxX, frustumCornersLightSpace[i].x);
		minY = fmin(minY, frustumCornersLightSpace[i].y);
		maxY = fmax(maxY, frustumCornersLightSpace[i].y);
		minZ = fmin(minZ, frustumCornersLightSpace[i].z);
		maxZ = fmax(maxZ, frustumCornersLightSpace[i].z);

	}

	// max and mins flipped
 	lightProjection = glm::ortho(minX, maxX, minY, maxY, -maxZ, -minZ);

	cascadeCorners[0] = glm::vec4(minX, minY, -maxZ, 1.0f);
	cascadeCorners[1] = glm::vec4(maxX, minY, -maxZ, 1.0f);
	cascadeCorners[2] = glm::vec4(minX, maxY, -maxZ, 1.0f);
	cascadeCorners[3] = glm::vec4(maxX, maxY, -maxZ, 1.0f);
	cascadeCorners[4] = glm::vec4(minX, minY, -minZ, 1.0f);
	cascadeCorners[5] = glm::vec4(maxX, minY, -minZ, 1.0f);
	cascadeCorners[6] = glm::vec4(minX, maxY, -minZ, 1.0f);
	cascadeCorners[7] = glm::vec4(maxX, maxY, -minZ, 1.0f);

	glm::vec3 dir = lightDirection;

 	for (unsigned int i = 0; i < 8; i++) {
		cascadeCorners[i] = glm::inverse(lightMat) * cascadeCorners[i];
		
		//cascadeCorners[i] = { cascadeCorners[i].y, cascadeCorners[i].x, cascadeCorners[i].z, cascadeCorners[i].w };     // light dir  1 -1  0
		//cascadeCorners[i] = { -cascadeCorners[i].y, -cascadeCorners[i].x, cascadeCorners[i].z, cascadeCorners[i].w };   // light dir -1 -1  0

		//cascadeCorners[i] = { cascadeCorners[i].x, cascadeCorners[i].z, cascadeCorners[i].y, cascadeCorners[i].w };     // light dir  0 -1  1
		//cascadeCorners[i] = { cascadeCorners[i].x, -cascadeCorners[i].z, -cascadeCorners[i].y, cascadeCorners[i].w };   // light dir  0 -1 -1
	
		cascadeCorners[i] = { cascadeCorners[i].x, cascadeCorners[i].z, cascadeCorners[i].y, cascadeCorners[i].w };
	}
}