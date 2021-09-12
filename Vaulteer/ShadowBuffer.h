#pragma once

#include <iostream>

#include "FrameBuffer.h"

/*
	Shadow buffer that uses a 2D texture to store depth data
*/
class ShadowBuffer : public FrameBuffer {
private:
	uint width, height;
public:
	ShadowBuffer(uint w, uint h);
	ShadowBuffer(ShadowBuffer&& mv) noexcept;

	~ShadowBuffer();

	uint initTexture(uint width, uint height);

};

