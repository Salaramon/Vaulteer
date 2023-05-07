#pragma once

#include <glm/glm.hpp>

class ShadowTechnique {
public:
	virtual void setModel(const glm::mat4& model) {
		//setUniform(Binder::shadow_vertex::uniforms::model, 1, GL_FALSE, model);
	}
	void setLightSpaceMatrix(const glm::mat4& lightSpaceMat) {
	    //setUniform(Binder::shadow_vertex::uniforms::lightSpaceMatrix, 1, GL_FALSE, lightSpaceMat);
	}
};
