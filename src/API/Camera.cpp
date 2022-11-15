#include "vpch.h"
#include "API/Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 direction, float roll, float renderDistance, float fov, float aspectRatio) :
	OR(this, DY::V(
		&lockedUp,
		&yaw,
		&pitch,
		&this->roll,
		&this->fov,
		&this->renderDistance,
		&this->aspectRatio,
		&orientation,
		&this->position), DY::N(
			"lockedUp",
			"yaw",
			"pitch",
			"roll",
			"fov",
			"renderDistance",
			"aspectRatio",
			"orientation",
			"position")),

	renderDistance(renderDistance),
	position(position),
	fov(fov),
	aspectRatio(aspectRatio),
	orientation(glm::vec3(.0f, .0f, .0f))
{
	OB.add(OR);
	LOG::CTOR::debug(this, DY::std_format("Created camera at position {}, poiting towards {}, with roll {}, render distance {}, fov {} and aspect ratio {}",
		DY::glm_to_string(position), DY::glm_to_string(glm::vec3(.0f, .0f, .0f)), roll, renderDistance, fov, aspectRatio));
}

Camera::Camera(float renderDistance, float fov, float aspectRatio) :
	OR(this, DY::V(
		&lockedUp,
		&yaw,
		&pitch,
		&this->roll,
		&this->fov,
		&this->renderDistance,
		&this->aspectRatio,
		&orientation,
		&this->position), DY::N(
			"lockedUp",
			"yaw",
			"pitch",
			"roll",
			"fov",
			"renderDistance",
			"aspectRatio",
			"orientation",
			"position")),

	renderDistance(renderDistance),
	fov(fov),
	aspectRatio(aspectRatio),
	orientation(glm::vec3(.0f, .0f, .0f))
{
	OB.add(OR);
	LOG::CTOR::debug(this, DY::std_format("Created camera poiting towards {}, render distance {}, fov {} and aspect ratio {}",
		DY::glm_to_string(glm::vec3(.0f, .0f, .0f)), renderDistance, fov, aspectRatio));
}

glm::mat4 Camera::getViewMatrix()
{
	glm::quat orientationConjugate = glm::conjugate(orientation);
	glm::mat4 rotation = glm::mat4_cast(orientationConjugate);
	glm::mat4 translation = glm::translate(glm::mat4(1.0), -position);
	glm::mat4 result = rotation * translation;

	LOG::CLAS::debug<&Camera::getViewMatrix>(this, DY::std_format("View matrix returned {}", DY::glm_to_string(result)));

	return result;
}

void Camera::move(glm::vec3 direction)
{
	position += direction;
	LOG::CLAS::debug<&Camera::move>(this, &position, DY::std_format("camera moved with {}, resulting in position {}", DY::glm_to_string(direction), DY::glm_to_string(position)));
}


void Camera::rotate(float yaw, float pitch, float roll)
{
	Camera::yaw = yaw;
	Camera::pitch = pitch;
	Camera::roll = roll;

	LOG::CLAS::debug<&Camera::rotate>(this, DY::std_format("camera to be rotated by {} yaw, {} pitch, {} roll", yaw, pitch, roll));
}

void Camera::setRotation(float yaw, float pitch, float roll)
{
	LOG::CLAS::debug<DY::OverloadSelector<Camera, void(float, float, float)>::Get<&Camera::setRotation>>(this, "this function is not implemented!");
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

	LOG::CLAS::debug<DY::OverloadSelector<Camera, void(glm::vec3, float)>::Get<&Camera::setRotation>>(this, "this function is not implemented!");
}

void Camera::lockUp(glm::vec3 fixedUp) {
	lockedUp = fixedUp;

	LOG::CLAS::debug<&Camera::lockUp>(this, &lockedUp, DY::std_format("up direction is locked to direction {}", DY::glm_to_string(lockedUp)));
}

void Camera::setPosition(float posX, float posY, float posZ)
{
	position = { posX, posY, posZ };

	LOG::CLAS::debug<&Camera::setPosition>(this, &position, DY::std_format("camera position set to {}", DY::glm_to_string(position)));
}

void Camera::apply()
{
	if (pitch != 0 || yaw != 0 || roll != 0) {
		glm::quat xRotation = glm::angleAxis(glm::radians(-pitch), glm::vec3(1, 0, 0));
		glm::quat yRotation = glm::angleAxis(glm::radians(-yaw), glm::vec3(0, 1, 0));
		glm::quat zRotation = glm::angleAxis(glm::radians(-roll), glm::vec3(0, 0, 1));

		if (lockedUp == glm::vec3(0.f)) {
			orientation *= xRotation * yRotation * zRotation;
		}
		else {
			glm::quat finalOrientation = orientation * xRotation * yRotation * zRotation;
			glm::quat finalFront = finalOrientation * glm::quat(0, 0, 0, -1) * glm::conjugate(finalOrientation);
			glm::vec3 frontVec = glm::normalize(glm::vec3(finalFront.x, finalFront.y, finalFront.z));
			glm::quat newOri = glm::quatLookAt(frontVec, lockedUp);
			orientation = newOri;
		}
	}

	LOG::CLAS::debug<&Camera::apply>([&]() {
		if (pitch != 0 || yaw != 0 || roll != 0) { 
			return true; }
		return false; },

		this, "camera rotation was updated!");
}

glm::mat4 Camera::getProjectionMatrix()
{
	glm::mat4 result = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, renderDistance);

	LOG::CLAS::debug<&Camera::getProjectionMatrix>(this, DY::std_format("Projection matrix returned {}", DY::glm_to_string(result)));

	return result;
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

	LOG::CLAS::debug<&Camera::getProjectionMatrix>(this, DY::std_format("Frustum returned left {}, right {} bottom {}, top {}, near {}, far {}",
		DY::glm_to_string(frustum.left),
		DY::glm_to_string(frustum.right),
		DY::glm_to_string(frustum.bottom),
		DY::glm_to_string(frustum.top),
		DY::glm_to_string(frustum.near),
		DY::glm_to_string(frustum.far)));
	
    return frustum;
}

glm::vec3 Camera::getPosition()
{
	LOG::CLAS::debug<&Camera::getPosition>(this, &position, DY::std_format("camera position returned {}", DY::glm_to_string(position)));

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

float Camera::getFov() {
	LOG::CLAS::debug<&Camera::getFov>(this, &fov, DY::std_format("camera fov returned {}", fov));
	return fov;
}

float Camera::getAspectRatio() {
	LOG::CLAS::debug<&Camera::getAspectRatio>(this, &aspectRatio, DY::std_format("camera aspect ratio returned {}", aspectRatio));
	return aspectRatio;
}

void Camera::setAspectRatio(int width, int height)
{
	aspectRatio = (float)width / (float)height;
	LOG::CLAS::debug<&Camera::setAspectRatio>(this, &aspectRatio, DY::std_format("camera aspect ratio is set to {}", aspectRatio));
}

glm::vec3 Camera::getFront()
{
	glm::quat qF = orientation * glm::quat(0, 0, 0, -1) * glm::conjugate(orientation);
	glm::vec3 result({ qF.x, qF.y, qF.z });

	LOG::CLAS::debug<&Camera::getFront>(this, DY::std_format("camera front vector was returned {}", DY::glm_to_string(result)));

	return result;
}

glm::vec3 Camera::getRight()
{
	glm::quat qF = orientation * glm::quat(0, -1, 0, 0) * glm::conjugate(orientation);
	//glm::rotate(glm::inverse(qF), 90.0f, glm::vec3({ qF.x, qF.y, qF.z })
	glm::vec3 result({ qF.x, qF.y, qF.z });

	LOG::CLAS::debug<&Camera::getFront>(this, DY::std_format("camera right vector was returned {}", DY::glm_to_string(result)));

	return result;
}

glm::vec3 Camera::getUp()
{
	glm::quat qF = orientation * glm::quat(0, 0, -1, 0) * glm::conjugate(orientation);
	glm::vec3 result({ qF.x, qF.y, qF.z });

	LOG::CLAS::debug<&Camera::getFront>(this, DY::std_format("camera up vector was returned {}", DY::glm_to_string(result)));

	return result;
}

