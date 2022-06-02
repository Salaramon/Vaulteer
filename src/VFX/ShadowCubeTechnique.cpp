#include "vpch.h"
#include "ShadowCubeTechnique.h"

void ShadowCubeTechnique::setModel(const glm::mat4& model) {
	setUniform(Binder::shadow_cube_vertex::uniforms::model, 1, GL_FALSE, model);
}

void ShadowCubeTechnique::setShadowMatrices(const std::vector<glm::mat4>& shadowMatrices) {
	setUniform(Binder::shadow_cube_geometry::uniforms::shadowMatrices[0], 6, GL_FALSE, *shadowMatrices.data());
}

void ShadowCubeTechnique::setLightPos(const glm::vec3& lightPos) {
	setUniform(Binder::shadow_cube_frag::uniforms::lightPos, 1, lightPos);
}

void ShadowCubeTechnique::setFarPlane(const float farPlane) {
	setUniform(Binder::shadow_cube_frag::uniforms::farPlane, farPlane);
}
