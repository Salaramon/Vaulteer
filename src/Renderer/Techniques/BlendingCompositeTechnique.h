#pragma once

class BlendingCompositeTechnique {
	using fragUnis = Binder::blending_composite_frag::uniforms;

public:
	static const Shader& shader() { return ShaderProgram::blendingCompositeShader(); }

	static void setTextureUnits(const GLint& accumulateTexture, const GLint& revealTexture) {
		shader().setUniform(fragUnis::accum, accumulateTexture);
		shader().setUniform(fragUnis::reveal, revealTexture);
	}
};
