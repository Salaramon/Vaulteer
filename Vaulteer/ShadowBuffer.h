#pragma once

#include <glad/glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <limits>

class ShadowBuffer
{
public:
	ShadowBuffer(unsigned int width, unsigned int height);
	ShadowBuffer(ShadowBuffer&& mv);

	~ShadowBuffer();

	bool init();

	void bindWrite();
	void bindRead();

	unsigned int shadowMapTexId;
	unsigned int width, height;
private:
	unsigned int FBO;
};

