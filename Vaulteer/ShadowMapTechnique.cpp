#include "ShadowMapTechnique.h"


void ShadowMapTechnique::setDepthMap(const GLuint depthMapTexId) {
    setUniform(Binder::shadow_map_frag::uniforms::depthMap, depthMapTexId);
}
