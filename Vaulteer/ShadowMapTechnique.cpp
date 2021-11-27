#include "ShadowMapTechnique.h"


void ShadowMapTechnique::setDepthMap(const GLuint depthMapTexId) {
    setUniform(Binder::depth_map_frag::uniforms::depthMap, depthMapTexId);
}
