#include "MyCamera.h"

MyCamera::MyCamera(glm::vec3 position, glm::vec3 cameraFront, glm::vec3 cameraUp, glm::vec3 cameraRight, float width, float height)
	: position(position), front(cameraFront), up(cameraUp), right(cameraRight), width(width), height(height)
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

glm::mat4 MyCamera::getStaticView()
{
	if (staticView == nullptr)
		return getViewMatrix();
	return *staticView;
}

glm::mat4 MyCamera::getStaticProjection()
{
	if (staticProjection == nullptr)
		return getProjectionMatrix();
	return *staticProjection;
}

glm::mat4 MyCamera::getViewMatrix() {
	return glm::lookAt(position, position + front, up);
}

glm::mat4 MyCamera::getProjectionMatrix() {
	//return glm::ortho(-16.f, 16.f, -10.f, 10.f, 0.1f, 250.0f);
	return glm::perspectiveFov(glm::radians(camera_fov), width, height, camera_near, camera_far);
}

void MyCamera::setViewport(float width, float height)
{
	this->width = width;
	this->height = height;
}

void MyCamera::updateRotation() {
	front.x = sin(glm::radians(-cameraYaw)) * cos(glm::radians(cameraPitch));
	front.y = sin(glm::radians(cameraPitch));
	front.z = cos(glm::radians(-cameraYaw)) * cos(glm::radians(cameraPitch));

	right = glm::normalize(glm::cross(front, { 0,1,0 }));
	up = glm::normalize(glm::cross(right, front));
}
