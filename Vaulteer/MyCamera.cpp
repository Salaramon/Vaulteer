#include "MyCamera.h"

MyCamera::MyCamera(glm::vec3 position, glm::vec3 cameraFront, glm::vec3 cameraUp, glm::vec3 cameraRight) 
	: position(position), front(cameraFront), up(cameraUp), right(cameraRight)
{
}

void MyCamera::move(glm::vec3 direction)
{
	position += direction;
}

void MyCamera::rotate(float yaw, float pitch)
{
	cameraYaw += yaw;
	cameraPitch += pitch;

	cameraPitch = glm::clamp(cameraPitch, -90.f + 0.01f, 90.f - 0.01f);

	updateRotation();
}

glm::mat4 MyCamera::getViewMatrix() {
	return glm::lookAt(position, position + front, up);
}

glm::mat4 MyCamera::getProjectionMatrix(float width, float height) {
	//return glm::ortho(-1.6f, 1.6f, -1.0f, 1.0f, 0.1f, 50.0f);
	return glm::perspectiveFov(glm::radians(camera_fov), width, height, .1f, 1000.0f);
}

void MyCamera::updateRotation() {
	front.x = sin(glm::radians(-cameraYaw)) * cos(glm::radians(cameraPitch));
	front.y = sin(glm::radians(cameraPitch));
	front.z = cos(glm::radians(-cameraYaw)) * cos(glm::radians(cameraPitch));

	right = glm::normalize(glm::cross(front, { 0,1,0 }));
	up = glm::normalize(glm::cross(right, front));
}
