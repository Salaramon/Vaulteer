#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

#include "FrameBuffer.h"
#include "LightTypes.h"

/*
	Shadow buffer that uses a cubemap to store depth data
*/
class ShadowCubeBuffer : public FrameBuffer {
public:
	ShadowCubeBuffer(const uint cubeSize, const glm::vec3 lightPosition, const float farPlane);
	ShadowCubeBuffer(const uint cubeSize, const GLSLPointLight pointLight);
	ShadowCubeBuffer(ShadowCubeBuffer&& mv) noexcept;

	~ShadowCubeBuffer();

	uint initTexture(uint width, uint height);

	std::vector<glm::mat4> getShadowTransforms();

	// determines texture size
	uint cubeSize;

	// determines buffer cube side transformations
	glm::vec3 lightPos;
	float farPlane;

private:
};

