#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class MyCamera
{
public:
	float camera_fov = 60.f;
	float camera_near = 0.1f, camera_far = 1000.0f;

	float cameraYaw = 0.f, cameraPitch = 0.f;
	glm::vec3 front, up, right;
	glm::vec3 position;
	float width, height;

	MyCamera(glm::vec3 position, glm::vec3 cameraFront, glm::vec3 cameraUp, glm::vec3 cameraRight, float width, float height);

	void move(glm::vec3 direction);
	void rotate(float yaw, float pitch);

	glm::mat4* staticView = nullptr;
	glm::mat4* staticProjection = nullptr;

	glm::mat4 getStaticView();
	glm::mat4 getStaticProjection();

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

	void setViewport(float width, float height);
private:

	void updateRotation();
};

