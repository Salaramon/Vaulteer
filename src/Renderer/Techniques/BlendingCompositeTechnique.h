#pragma once

#include "Renderer/Buffers/UniformBuffer.h"

class BlendingCompositeTechnique {
	using fragUnis = Binder::blending_composite_frag::uniforms;
public:
	static void setTextureUnits(const GLint& accumulateTexture, const GLint& revealTexture);
};
