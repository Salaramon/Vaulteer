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

	Camera(glm::vec3 position, glm::vec3 direction, float roll, float renderDistance, float fov, float aspectRatio, Binder::UniformInfo view, Binder::UniformInfo projection);
	Camera(float renderDistance, float fov, float aspectRatio, Binder::UniformInfo view, Binder::UniformInfo projection);

	void setShaderContext(Shader* shader);

	void move(glm::vec3 direction);
	void rotate(float yaw, float pitch, float roll);
	void setRotation(float yaw, float pitch, float roll);
	void setRotation(glm::vec3 direction, float roll);
	void setPosition(float posX, float posY, float posZ);

	void updateShaderMatrices();

private:
	float fov;
	float renderDistance;
	float aspectRatio;

	glm::mat4 mView;
	glm::mat4 mProjection;

	Binder::UniformInfo uView;
	Binder::UniformInfo uProjection;

	Shader* shader;

	glm::quat orientation;
	glm::vec3 position = { 0,0,0 };
	float angleUp = 0;
	float angleRight = 0;
	float angleFront = 0;

	std::string getOrientation();
	glm::vec3 getFront();
	glm::vec3 getRight();
	glm::vec3 getUp();

	glm::mat4 GetViewMatrix();

	void updateRotation();
};
