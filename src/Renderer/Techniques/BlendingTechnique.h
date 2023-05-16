#pragma once

#include "Renderer/Buffers/UniformBuffer.h"
#include "Model/Resources/MaterialLibrary.h"
#include "Model/Model.h"
#include "Renderer/ShaderProgram.h"
#include "Scene/Scene.h"

class BlendingTechnique {
public:
	static const Shader& shader() { return ShaderProgram::blendingShader(); }

	static void setModel(const glm::dmat4& model) {
		shader().setUniform(Binder::blending_vertex::uniforms::model, 1, GL_FALSE, model);
		setNormal(glm::inverse(model));
	}

	static void setView(const glm::dmat4& view) {
		shader().setUniform(Binder::blending_vertex::uniforms::view, 1, GL_FALSE, view);
	}

	static void setNormal(const glm::mat4& normal) {
		shader().setUniform(Binder::blending_vertex::uniforms::normal, 1, GL_FALSE, normal);
	}

	static void setInverseViewMatrix(const glm::mat4& view) {
		shader().setUniform(Binder::blending_frag::uniforms::inverseViewMat, 1, GL_FALSE, view);
	}

	static void setTextureUnit(const GLint& texture) {
		shader().setUniform(Binder::blending_frag::uniforms::textureLib, texture);
	}
};
