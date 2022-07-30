#include "vpch.h"
#include "ShadowCubeBuffer.h"


ShadowCubeBuffer::ShadowCubeBuffer(const uint cubeSize, const glm::vec3 lightPosition, const float farPlane) : TextureFrameBuffer(initTexture(cubeSize, cubeSize)),
cubeSize(cubeSize), lightPos(lightPosition), farPlane(farPlane)
{}


ShadowCubeBuffer::ShadowCubeBuffer(const uint cubeSize, const PointLight& pointLight) : TextureFrameBuffer(initTexture(cubeSize, cubeSize)),
	cubeSize(cubeSize), lightPos(pointLight.position), farPlane(pointLight.calculatePointRadius())
{}

ShadowCubeBuffer::ShadowCubeBuffer(ShadowCubeBuffer&& mv) noexcept :
	TextureFrameBuffer(std::move(mv)),
	cubeSize(mv.cubeSize),
	lightPos(mv.lightPos),
	farPlane(mv.farPlane)
{}

ShadowCubeBuffer::~ShadowCubeBuffer() {
	destroy();
}

uint ShadowCubeBuffer::initTexture(uint width, uint height) {
	uint texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texId);

	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
			width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return texId;
}

std::vector<glm::mat4> ShadowCubeBuffer::getShadowTransforms() {
	std::vector<glm::mat4> shadowTransforms;

	float nearPlane = 1.0f;
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, nearPlane, farPlane);

	shadowTransforms.push_back(shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj *
		glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

 	return shadowTransforms;
}
