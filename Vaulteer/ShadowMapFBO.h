#pragma once

#include <glad/glad.h>
#include <glfw3.h>

#include <iostream>

class ShadowMapFBO
{
public:
	ShadowMapFBO();
	~ShadowMapFBO();

	bool init(unsigned int width, unsigned int height);

	void bindWrite();
	void bindRead(GLenum TextureUnit);

private:

	unsigned int FBO;
	unsigned int shadowMapTexId;
};

