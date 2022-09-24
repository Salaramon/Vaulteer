#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include <string>
#include <array>

#include "Debug/DebugLogger.h"

class Camera : public DebugLogger<Camera>
{
public:

	struct Frustum {
		explicit Frustum(std::array<glm::vec4, 6> arr) : 
			left(arr[0]),
			right(arr[1]),
			bottom(arr[2]),
			top(arr[3]),
			near(arr[4]),
			far(arr[5])
		{}
		glm::vec4 left;
		glm::vec4 right;
		glm::vec4 bottom;
		glm::vec4 top;
		glm::vec4 near;
		glm::vec4 far;
	};

	Camera();
	Camera(glm::vec3 position, glm::vec3 direction, float roll, float renderDistance, float fov, float aspectRatio);
	Camera(float renderDistance, float fov, float aspectRatio);

	void move(glm::vec3 direction);
	void rotate(float yaw, float pitch, float roll);
	void setRotation(float yaw, float pitch, float roll);
	void setRotation(glm::vec3 direction, float roll);
	void lockUp(glm::vec3 fixedUp);
	void setPosition(float posX, float posY, float posZ);

	void apply();

	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix() const;
	Frustum getFrustum() const;
	glm::vec3 getPosition() const;

	std::string getOrientation() const;
	glm::vec3 getFront() const;
	glm::vec3 getRight() const;
	glm::vec3 getUp() const;
	float getFov() const;

	float getAspectRatio() const;
	void setAspectRatio(int width, int height);

private:
	glm::vec3 lockedUp = glm::vec3(0.0f);

	float yaw{};
	float pitch{};
	float roll{};

	float fov;
	float renderDistance;
	float aspectRatio;

	glm::quat orientation;
	glm::vec3 position = { 0,0,0 };
};
