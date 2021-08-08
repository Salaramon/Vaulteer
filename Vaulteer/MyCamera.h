#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class MyCamera
{
public:
	float camera_fov = 40.f;
	float cameraYaw = 0.f, cameraPitch = 0.f;
	glm::vec3 front, up, right;
	glm::vec3 position;

	MyCamera(glm::vec3 position, glm::vec3 cameraFront, glm::vec3 cameraUp, glm::vec3 cameraRight);

	void move(glm::vec3 direction);
	void rotate(float yaw, float pitch);

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix(float width, float height);
private:



	void updateRotation();
};

