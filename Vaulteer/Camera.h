#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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
	void setPosition(float posX, float posY, float posZ);

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	glm::vec3 getPosition();

private:
	float fov;
	float renderDistance;
	float aspectRatio;

	glm::mat4 mView;
	glm::mat4 mProjection;

	glm::quat orientation;
	glm::vec3 position = { 0,0,0 };
	float angleUp = 0;
	float angleRight = 0;
	float angleFront = 0;

	std::string getOrientation();
	glm::vec3 getFront();
	glm::vec3 getRight();
	glm::vec3 getUp();

	void updateRotation();
};
