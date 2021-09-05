#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 direction, float roll, float renderDistance, float fov, float aspectRatio, Binder::UniformInfo view, Binder::UniformInfo projection) :
	renderDistance(renderDistance),
	position(position),
	fov(fov),
	aspectRatio(aspectRatio),
	uView(view),
	uProjection(projection)
{
	setRotation(direction, roll);
	updateRotation();

	debug("Camera created at: " + glm::to_string(position) + "\n");
}

Camera::Camera(float renderDistance, float fov, float aspectRatio, Binder::UniformInfo view, Binder::UniformInfo projection) :
	renderDistance(renderDistance),
	fov(fov),
	aspectRatio(aspectRatio),
	uView(view),
	uProjection(projection)
{
	updateRotation();

	debug("Camera created at: " + glm::to_string(position) + "\n");
}

glm::mat4 Camera::GetViewMatrix()
{
	glm::quat orientationConjugate = glm::conjugate(orientation);
	glm::mat4 rotation = glm::mat4_cast(orientationConjugate);
	glm::mat4 translation = glm::translate(glm::mat4(1.0), -position);

	return rotation * translation;
}

void Camera::setShaderContext(Shader* shader)
{
	Camera::shader = shader;
}

void Camera::move(glm::vec3 direction)
{
	position += direction;
}


void Camera::rotate(float yaw, float pitch, float roll)
{
	angleUp += pitch;
	angleRight += yaw;
	angleFront += roll;

	updateRotation();
}

void Camera::setRotation(float yaw, float pitch, float roll)
{
	angleUp = pitch;
	angleRight = yaw;
	angleFront = roll;

	updateRotation();
	
}

void Camera::setRotation(glm::vec3 direction, float roll) {
	
	orientation = glm::quatLookAt(glm::normalize(direction), getUp());
	angleFront = roll;

	updateRotation();
}

void Camera::setPosition(float posX, float posY, float posZ)
{
}

void Camera::updateRotation() {
	glm::quat xRotation = glm::angleAxis(glm::radians(-angleUp), glm::vec3(1, 0, 0));
	glm::quat yRotation = glm::angleAxis(glm::radians(-angleRight), glm::vec3(0, 1, 0));
	glm::quat zRotation = glm::angleAxis(glm::radians(-angleFront), glm::vec3(0, 0, 1));

	orientation *= xRotation * yRotation * zRotation;

	angleUp = 0.0f;
	angleRight = 0.0f;
	angleFront = 0.0f;
}

void Camera::updateShaderMatrices()
{
	//can be optimized by only calling glm::perspective and GetViewMatrix only when changes happened.
	mProjection =  glm::perspective(glm::radians(fov), aspectRatio, 0.1f, renderDistance);
	shader->setUniform(Binder::vertex::uniforms::projection, 1, GL_FALSE, mProjection);

	mView = GetViewMatrix();
	shader->setUniform(Binder::vertex::uniforms::view, 1, GL_FALSE, mView);
}

std::string Camera::getOrientation() {
	return std::string("[" + std::to_string(orientation.x) + "," + std::to_string(orientation.y) + "," + std::to_string(orientation.z) + "," + std::to_string(orientation.w) + "," + "]");
}

glm::vec3 Camera::getFront()
{
	glm::quat qF = orientation * glm::quat(0, 0, 0, -1) * glm::conjugate(orientation);
	return glm::vec3({ qF.x, qF.y, qF.z });
}

glm::vec3 Camera::getRight()
{
	glm::quat qF = orientation * glm::quat(0, -1, 0, 0) * glm::conjugate(orientation);
	//glm::rotate(glm::inverse(qF), 90.0f, glm::vec3({ qF.x, qF.y, qF.z })
	return glm::vec3({ qF.x, qF.y, qF.z });
}

glm::vec3 Camera::getUp()
{
	glm::quat qF = orientation * glm::quat(0, 0, -1, 0) * glm::conjugate(orientation);
	return glm::vec3({ qF.x, qF.y, qF.z });
}

