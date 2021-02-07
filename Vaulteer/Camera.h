#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "Event.h"


class Camera
{
public:
	// camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// euler Angles
	float yaw;
	float pitch;
	// camera options
	float speed;
	float Zoom;

	// constructor with vectors
	Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);
	// constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	// returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix();

	void move(glm::vec3 direction);
	void rotate(glm::vec2 direction);

private:
	// calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors();

};
