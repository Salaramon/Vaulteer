#pragma once

#include <vector>
#include <glm/glm.hpp>

class ShadowCubeTechnique {
public:

	void setModel(const glm::mat4& model) {
		//setUniform(Binder::shadow_cube_vertex::uniforms::model, 1, GL_FALSE, model);
	}

	void setShadowMatrices(const std::vector<glm::mat4>& shadowMatrices) {
		//setUniform(Binder::shadow_cube_geometry::uniforms::shadowMatrices[0], 6, GL_FALSE, *shadowMatrices.data());
	}

	void setLightPos(const glm::vec3& lightPos) {
		//setUniform(Binder::shadow_cube_frag::uniforms::lightPos, 1, lightPos);
	}

	void setFarPlane(const float farPlane) {
		//setUniform(Binder::shadow_cube_frag::uniforms::farPlane, farPlane);
	}

};

