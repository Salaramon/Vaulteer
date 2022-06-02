#include "vpch.h"
#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 direction, float roll, float renderDistance, float fov, float aspectRatio) :
	renderDistance(renderDistance),
	position(position),
	fov(fov),
	aspectRatio(aspectRatio),
	orientation(glm::vec3(.0f, .0f, .0f))
{
	//setRotation(direction, roll);

	debug("Camera created at: " + glm::to_string(position) + "\n");
}

Camera::Camera(float renderDistance, float fov, float aspectRatio) :
	renderDistance(renderDistance),
	fov(fov),
	aspectRatio(aspectRatio),
	orientation(glm::vec3(.0f, .0f, .0f))
{

	debug("Camera created at: " + glm::to_string(position) + "\n");
}

glm::mat4 Camera::getViewMatrix()
{
	glm::quat orientationConjugate = glm::conjugate(orientation);
	glm::mat4 rotation = glm::mat4_cast(orientationConjugate);
	glm::mat4 translation = glm::translate(glm::mat4(1.0), -position);

	return rotation * translation;
}

void Camera::move(glm::vec3 direction)
{
	position += direction;
}


void Camera::rotate(float yaw, float pitch, float roll)
{
	Camera::yaw = yaw;
	Camera::pitch = pitch;
	Camera::roll = roll;
}

void Camera::setRotation(float yaw, float pitch, float roll)
{
	
	
}

void Camera::setRotation(glm::vec3 direction, float roll) {
	/*
	glm::vec3 up = getUp();
	if (std::abs(glm::dot(up, direction)) == 1) {
		up.x = (bool)up.x ? 0 : 1;
		up.y = (bool)up.y ? 0 : 1;
		up.z = (bool)up.z ? 0 : 1;
		glm::normalize(up);
	}

	orientation = glm::quatLookAt(glm::normalize(direction), up);
	*/
	//orientation *= glm::angleAxis(glm::radians(-roll), glm::vec3(0, 0, 1));
}

void Camera::lockUp(glm::vec3 fixedUp) {
	lockedUp = fixedUp;
}

void Camera::setPosition(float posX, float posY, float posZ)
{
	position = { posX, posY, posZ };
}

void Camera::apply()
{
	if (pitch != 0 || yaw != 0 || roll != 0) {
		glm::quat xRotation = glm::angleAxis(glm::radians(-pitch), glm::vec3(1, 0, 0));
		glm::quat yRotation = glm::angleAxis(glm::radians(-yaw), glm::vec3(0, 1, 0));
		glm::quat zRotation = glm::angleAxis(glm::radians(-roll), glm::vec3(0, 0, 1));

		if (lockedUp == glm::vec3(0.f)) {
			orientation *= xRotation* yRotation* zRotation;
		}
		else {
			glm::quat finalOrientation = orientation * xRotation * yRotation * zRotation;
			glm::quat finalFront = finalOrientation * glm::quat(0, 0, 0, -1) * glm::conjugate(finalOrientation);
			glm::vec3 frontVec = glm::normalize(glm::vec3(finalFront.x, finalFront.y, finalFront.z));
			glm::quat newOri = glm::quatLookAt(frontVec, lockedUp);
			orientation = newOri;
		}
	}
}

glm::mat4 Camera::getProjectionMatrix()
{
	return glm::perspective(glm::radians(fov), aspectRatio, 0.1f, renderDistance);
}

Camera::Frustum Camera::getFrustum()
{
	std::array<glm::vec4, 6> frustumArray;
	glm::mat4 projectionMatrix = getProjectionMatrix();

	std::array<std::function<size_t(const size_t&, const size_t&)>, 2> op = { std::plus<size_t>(), std::minus<size_t>() };
	for (size_t i = 0; i < frustumArray.size(); i++) {
		for (size_t j = 0; j < 4; j++) {
			frustumArray[i][j] = op[i%2](projectionMatrix[j][3], projectionMatrix[j][i%3]);
		}
	}

	Frustum frustum = Frustum(frustumArray);
	
    return frustum;
}

glm::vec3 Camera::getPosition()
{
	return position;
}

/*
void Camera::updateShaderMatrices()
{
	//can be optimized by only calling glm::perspective and GetViewMatrix only when changes happened.
	mProjection =  glm::perspective(glm::radians(fov), aspectRatio, 0.1f, renderDistance);
	shader->setUniform(uProjection, 1, GL_FALSE, mProjection);

	mView = GetViewMatrix();
	shader->setUniform(uView, 1, GL_FALSE, mView);
}
*/

std::string Camera::getOrientation() {
	return std::string("[" + std::to_string(orientation.x) + "," + std::to_string(orientation.y) + "," + std::to_string(orientation.z) + "," + std::to_string(orientation.w) + "," + "]");
}

float Camera::getFov() {
	return fov;
}

float Camera::getAspectRatio() {
	return aspectRatio;
}

void Camera::setAspectRatio(int width, int height)
{
	aspectRatio = (float)width / (float)height;
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

