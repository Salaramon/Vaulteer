#include "ShadowCascade.h"

ShadowCascade::ShadowCascade(float zNear, float zFar) : 
	zNear(zNear), zFar(zFar), 
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
	float fov = camera.camera_fov;
	float aspect_ratio_v = camera.height / camera.width;
	float tanHalfHFOV = tanf(std::ctradians(fov / 2.0f));
	float tanHalfVFOV = tanf(std::ctradians((fov * aspect_ratio_v) / 2.0f));


	float xn = zNear * tanHalfHFOV;
	float yn = zNear * tanHalfVFOV;
	float xf = zFar * tanHalfHFOV;
	float yf = zFar * tanHalfVFOV;

	// near plane
	frustumCorners[0] = glm::vec4(-xn, -yn, zNear, 1.0f);
	frustumCorners[1] = glm::vec4(xn, -yn, zNear, 1.0f);
	frustumCorners[2] = glm::vec4(-xn, yn, zNear, 1.0f);
	frustumCorners[3] = glm::vec4(xn, yn, zNear, 1.0f);
	// far plane
	frustumCorners[4] = glm::vec4(-xf, -yf, zFar, 1.0f);
	frustumCorners[5] = glm::vec4(xf, -yf, zFar, 1.0f);
	frustumCorners[6] = glm::vec4(-xf, yf, zFar, 1.0f);
	frustumCorners[7] = glm::vec4(xf, yf, zFar, 1.0f);

	glm::vec3 center((xf - xn)/2, (yf - yn)/2, (zFar - zNear)/2);
	glm::mat4 lightMat = glm::lookAt(glm::vec3(0.f), lightDirection, glm::vec3(0.f, 1.f, 0.f));
	lightView = lightMat;

	camera.front = -camera.front;
	glm::mat4 cameraInverseMat = glm::inverse(camera.getViewMatrix());
	camera.front = -camera.front;

	glm::vec4 frustumCornersLightSpace[8];
	float minX = std::numeric_limits<float>::max();
	float maxX = -std::numeric_limits<float>::max();
	float minY = std::numeric_limits<float>::max();
	float maxY = -std::numeric_limits<float>::max();
	float minZ = std::numeric_limits<float>::max();
	float maxZ = -std::numeric_limits<float>::max();

	cameraFrustumWS.clear();
	for (unsigned int i = 0; i < 8; i++) {
		// Transform the frustum coordinate from view to world space
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

	//lightProjection = glm::ortho(minX, maxX, -maxY, -minY, minZ, maxZ);
 	lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
	//lightProjection = glm::ortho(-10.f, 10.f, minY, maxY, -10.f, 10.f);

	minX = std::numeric_limits<float>::max();
	maxX = -std::numeric_limits<float>::max();
	minY = std::numeric_limits<float>::max();
	maxY = -std::numeric_limits<float>::max();
	minZ = std::numeric_limits<float>::max();
	maxZ = -std::numeric_limits<float>::max();
	for (unsigned int i = 0; i < 8; i++) {
		// Transform the frustum coordinate from view to world space
		frustumCornersLightSpace[i] = cameraInverseMat * frustumCorners[i];

		minX = fmin(minX, frustumCornersLightSpace[i].x);
		maxX = fmax(maxX, frustumCornersLightSpace[i].x);
		minY = fmin(minY, frustumCornersLightSpace[i].y);
		maxY = fmax(maxY, frustumCornersLightSpace[i].y);
		minZ = fmin(minZ, frustumCornersLightSpace[i].z);
		maxZ = fmax(maxZ, frustumCornersLightSpace[i].z);
	}
	bounds = { minX, maxX, minY, maxY, minZ, maxZ };

	//camera.staticView = &lightView;
	//camera.staticProjection = &lightProjection;
}

/*void ShadowCascade::updateBounds(MyCamera& camera, glm::vec3 lightDirection) {
	glm::mat4 inverseMat = glm::inverse(camera.getViewMatrix());
	glm::vec3 center(0.0f);

	glm::mat4 newView = glm::lookAt(lightDirection, center, glm::vec3(0.f, 1.f, 0.f));


	float minZ = std::numeric_limits<float>::max();
	float maxZ = -std::numeric_limits<float>::max();
	for (int i = 0; i < 8; i++) {
		glm::vec4 corner = frustumCorners[i];
		corner = inverseMat * corner;
		corner /= corner.w;

		center = center + (glm::vec3(corner.x, corner.y, corner.z) / 8.0f);
		minZ = fmin(minZ, corner.z);
		maxZ = fmax(maxZ, corner.z);
	}

	float distance = maxZ - minZ;
	glm::vec3 incomingLightPos = -lightDirection * distance;
	glm::vec3 lightPosition = center + incomingLightPos;

	// update view
	setView(newView);
	//camera.staticView = &lightView;
	//camera.position = incomingLightPos;


	// update projection
	glm::vec3 projMins(std::numeric_limits<float>::max());
	glm::vec3 projMaxs(-std::numeric_limits<float>::max());

	for (int i = 0; i < 8; i++) {
		glm::vec4 corner = frustumCorners[i];
		corner = newView * corner;

		projMins.x = fmin(corner.x, projMins.x);
		projMaxs.x = fmax(corner.x, projMaxs.x);
		projMins.y = fmin(corner.y, projMins.y);
		projMaxs.y = fmax(corner.y, projMaxs.y);
		projMins.z = fmin(corner.z, projMins.z);
		projMaxs.z = fmax(corner.z, projMaxs.z);
	}
	float zDist = projMaxs.z - projMins.z;

	float near_plane = 0.1f, far_plane = 50.0f;
	glm::mat4 newProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	//setProjection(newProjection);

 	setProjection(glm::ortho(projMins.x, projMaxs.x, projMins.y, projMaxs.y, 0.0f, zDist));
	glm::mat4 tmpProj = glm::perspectiveFov(glm::radians(camera.camera_fov), camera.width, camera.height, 0.1f, 1000.0f);

	//camera.staticProjection = &lightProjection;
}
*/

