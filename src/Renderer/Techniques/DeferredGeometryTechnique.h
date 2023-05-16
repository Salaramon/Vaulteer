#pragma once

#include "Model/Model.h"
#include "Renderer/ShaderProgram.h"
#include "Scene/Scene.h"
#include "Renderer/Buffers/UniformBuffer.h"

class DeferredGeometryTechnique {
public:
	static const Shader& shader() { return ShaderProgram::geometryShader(); }

	static void setModel(const glm::dmat4& model) {
		shader().setUniform(Binder::geometry_vertex::uniforms::model, 1, GL_FALSE, model);
	}

	static void setModelView(const glm::dmat4& model, const glm::dmat4& view) {
		glm::fmat4 modelView = view * model;
		shader().setUniform(Binder::geometry_vertex::uniforms::modelView, 1, GL_FALSE, modelView);
		setModel(model);
		setNormal(model);
	}

	static void setNormal(const glm::mat4& normal) {
		shader().setUniform(Binder::geometry_vertex::uniforms::normal, 1, GL_FALSE, glm::transpose(glm::inverse(normal)));
	}

	static void setTextureUnit(const GLint& texture) {
		shader().setUniform(Binder::geometry_frag::uniforms::textureLib, texture);
	}

};

