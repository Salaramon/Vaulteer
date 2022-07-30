#pragma once

#include <iostream>

#include "VFX/LightTypes.h"
#include "Renderer/Buffers/TextureFrameBuffer.h"

/*
	Shadow buffer that uses a 2D texture to store depth data
*/
class ShadowBuffer : public TextureFrameBuffer {
private:
public:
	uint width, height;

	ShadowBuffer(uint w, uint h);
	ShadowBuffer(uint size);
	ShadowBuffer(const SpotLight& spotLight);
	ShadowBuffer(ShadowBuffer&& mv) noexcept;

	~ShadowBuffer();

	uint initTexture(uint width, uint height);
};

