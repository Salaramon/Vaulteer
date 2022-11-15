#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp >
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include <vector>
#include <string>
#include <array>
#include <functional>

#include "API/Event.h"

#include "Debug/Debug.h"

class Camera
{
public:

	struct Frustum {
	public:
		Frustum(std::array<glm::vec4, 6> arr) : 
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
	Frustum getFrustum();
	glm::vec3 getPosition();

	glm::vec3 getFront();
	glm::vec3 getRight();
	glm::vec3 getUp();

	float getFov();
	float getAspectRatio();
	void setAspectRatio(int width, int height);

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

public:

	inline static auto CR = DY::ClassRegister<
		&move,
		&rotate,
		DY::OverloadSelector<Camera, void(float, float, float)>::Get<&Camera::setRotation>,
		DY::OverloadSelector<Camera, void(glm::vec3, float)>::Get<&Camera::setRotation>,
		&lockUp,
		&setPosition,
		&apply,
		&getViewMatrix,
		&getProjectionMatrix,
		&getFrustum,
		&getPosition,
		&getFront,
		&getRight,
		&getUp,
		&getFov,
		&getAspectRatio,
		&setAspectRatio

	>(	"lockUp",
		"setPosition",
		"apply",
		"getViewMatrix",
		"getProjectionMatrix",
		"getFrustum",
		"getPosition",
		"getFront",
		"getRight",
		"getUp",
		"getFov",
		"getAspectRatio",
		"setAspectRatio");

	DY::ObjectRegister<Camera,
		decltype(lockedUp),
		decltype(yaw),
		decltype(pitch),
		decltype(roll),
		decltype(fov),
		decltype(renderDistance),
		decltype(aspectRatio),
		decltype(orientation),
		decltype(position)> OR;

	inline static auto CB = DY::ClassBinder<decltype(CR)>();
	inline static auto OB = DY::ObjectBinder<decltype(OR)>();


	using LOG = _LOG<decltype(CB), decltype(OB), DY::No_FB, DY::No_VB>;
};
