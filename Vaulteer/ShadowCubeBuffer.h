#pragma once

#include <iostream>

#include "FrameBuffer.h"

/*
	Shadow buffer that uses a cubemap to store depth data
*/
class ShadowCubeBuffer : public FrameBuffer {
public:
	ShadowCubeBuffer(const uint cubeSize);
	ShadowCubeBuffer(ShadowCubeBuffer&& mv) noexcept;

	~ShadowCubeBuffer();

	uint initTexture(uint width, uint height);

private:
	uint cubeSize;
};

