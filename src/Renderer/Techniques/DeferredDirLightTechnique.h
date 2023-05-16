#pragma once

#include "Model/Material.h"
#include "Renderer/ShaderProgram.h"
#include "Renderer/Buffers/UniformBuffer.h"

#include "VFX/LightTypes.h"

class DeferredDirLightTechnique {
public:
	using dirVertUnis = Binder::deferred_directional_vertex::uniforms;
	using dirFragUnis = Binder::deferred_directional_frag::uniforms;

	static const Shader& shader() { return ShaderProgram::deferredDirShader(); }

	static void setWorldCameraPos(const glm::vec3& cameraPos) {
	    shader().setUniform(dirFragUnis::worldCameraPos, 1, cameraPos);
	}

	static void setCameraViewMat(const glm::mat4& view) {
	    shader().setUniform(dirFragUnis::cameraViewMat, 1, GL_FALSE, view);
	}

	static void setFogColor(const glm::vec3& color) {
	    shader().setUniform(dirFragUnis::fogColor, 1, color);
	}

};

