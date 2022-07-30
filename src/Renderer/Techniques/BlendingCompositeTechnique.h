#pragma once

#include "Renderer/ShaderProgram.h"
#include "Renderer/Buffers/UniformBuffer.h"

class BlendingCompositeTechnique : public ShaderProgram<BlendingCompositeShader>
{
	using fragUnis = Binder::blending_composite_frag::uniforms;
public:
	static void setTextureUnits(const GLint& accumulateTexture, const GLint& revealTexture);
};

