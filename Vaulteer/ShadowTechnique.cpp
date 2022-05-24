#include "vpch.h"
#include "ShadowTechnique.h"


void ShadowTechnique::setModel(const glm::mat4& model) {
    setUniform(Binder::shadow_vertex::uniforms::model, 1, GL_FALSE, model);
}

void ShadowTechnique::setLightSpaceMatrix(const glm::mat4& lightSpaceMat) {
    setUniform(Binder::shadow_vertex::uniforms::lightSpaceMatrix, 1, GL_FALSE, lightSpaceMat);
}

/*void ShadowTechnique::setShadowMap(GLuint textureUnit) {
    setUniform(Binder::depth_frag::uniforms::shadowMap, textureUnit);
}*/