#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp >
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include <vector>
#include <string>

#include "Event.h"
#include "Shader.h"

#include "DebugLogger.h"

class Camera : public DebugLogger<Camera>
{
public:

	Camera(glm::vec3 position, glm::vec3 direction, float roll, float renderDistance, float fov, float aspectRatio);
	Camera(float renderDistance, float fov, float aspectRatio);

	void move(glm::vec3 direction);
	void rotate(float yaw, float pitch, float roll);
	void setRotation(float yaw, float pitch, float roll);
	void setRotation(glm::vec3 direction, float roll);
	void lockUp(glm::vec3 fixedUp);
	void setPosition(float posX, float posY, float posZ);

	void apply();

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	glm::mat4 getFrustumMatrix();
	glm::vec3 getPosition();

	std::string getOrientation();
	glm::vec3 getFront();
	glm::vec3 getRight();
	glm::vec3 getUp();

private:
	glm::vec3 lockedUp = glm::vec3(0.0f);

	float yaw;
	float pitch;
	float roll;

	float fov;
	float renderDistance;
	float aspectRatio;

	glm::quat orientation;
	glm::vec3 position = { 0,0,0 };
};
