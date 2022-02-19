#include "DepthMapTechnique.h"


void DepthMapTechnique::setDepthMap(const GLuint depthMapTexId) {
    setUniform(Binder::depth_map_frag::uniforms::depthMap, depthMapTexId);
}
