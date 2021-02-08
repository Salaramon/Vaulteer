#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <vector>
#include <string>

#include "Event.h"


class Camera
{
public:

	// euler Angles
	double_t yaw;
	double_t pitch;
	double_t roll;

	glm::quat orientation;
	glm::vec3 position;
	float angleUp;
	float angleRight;
	float angleFront;

	// constructor with vectors
	Camera(glm::vec3 position, float yaw, float pitch, float roll);
	// constructor with scalar values
	Camera(float posX, float posY, float posZ, float yaw, float pitch, float roll);

	// returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix();

	void move(glm::vec3 direction);
	void rotate(float yaw, float pitch, float roll);

	std::string getOrientation();
	glm::vec3 getFront();
	glm::vec3 getRight();
	glm::vec3 getUp();

private:
	void updateRotation();
};
