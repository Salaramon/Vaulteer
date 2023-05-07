#pragma once

#include "Renderer/ShaderProgram.h"
#include "Renderer/Buffers/UniformBuffer.h"

class ShadowVolumeTechnique {
public:
	static const Shader& shader() { return ShaderProgram::shadowVolumeShader(); }

	static void setModelView(const glm::dmat4& model, const glm::dmat4& view);
	static void setInverseViewMatrix(const glm::mat4& view);

	// uniform buffer upload
	// TODO do whatever... i guess... baka

	static void uploadProjection(const glm::mat4& projection);

	static UniformBuffer& getUBCamera() {
		static UniformBuffer camera = UniformBuffer(Binder::forward_vertex::buffers::Camera);
		return camera;
	}
};

inline void ShadowVolumeTechnique::setModelView(const glm::dmat4& model, const glm::dmat4& view) {
	glm::fmat4 modelView = view * model;
	shader().setUniform(Binder::volume_vertex::uniforms::modelView, 1, GL_FALSE, modelView);
}

inline void ShadowVolumeTechnique::uploadProjection(const glm::mat4& projection) {
	auto& camera = getUBCamera();
	UniformBuffer::insert(camera, projection);
}