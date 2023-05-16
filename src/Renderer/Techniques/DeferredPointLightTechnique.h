#pragma once

#include "Renderer/Buffers/UniformBuffer.h"

#include "VFX/LightTypes.h"
#include "Model/Resources/MaterialLibrary.h"
#include "Renderer/ShaderProgram.h"

class DeferredPointLightTechnique {
public:
	using vertUnis = Binder::deferred_point_vertex::uniforms;
	using fragUnis = Binder::deferred_point_frag::uniforms;
	
	static const Shader& shader() { return ShaderProgram::deferredPointShader(); }


	static void setPointLightIndex(const int index) {
		shader().setUniform(fragUnis::pointLightIndex, index);
	}

	static void setModel(const glm::dmat4& model) {
		shader().setUniform(vertUnis::model, 1, GL_FALSE, model);
	}

	static void setView(const glm::dmat4& view) {
		shader().setUniform(vertUnis::view, 1, GL_FALSE, view);
	}

	static void setWorldCameraPos(const glm::vec3& cameraPos) {
		shader().setUniform(fragUnis::worldCameraPos, 1, cameraPos);
	}

	static void setCameraViewMat(const glm::mat4& view) {
		shader().setUniform(fragUnis::cameraViewMat, 1, GL_FALSE, view);

		//setInverseViewMat(glm::inverse(view));
	}

};

