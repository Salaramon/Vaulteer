#pragma once

#include "Renderer/ShaderProgram.h"
#include "Renderer/Buffers/UniformBuffer.h"

class ShadowVolumeTechnique {
public:
	static const Shader& shader() { return ShaderProgram::shadowVolumeShader(); }

	static void setModelView(const glm::dmat4& model, const glm::dmat4& view) {
		// TODO
	}

	static void setInverseViewMatrix(const glm::mat4& view) {
		
	}
};