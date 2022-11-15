#include "vpch.h"
#include "BlendingCompositeTechnique.h"

void BlendingCompositeTechnique::setTextureUnits(const GLint& accumulateTexture, const GLint& revealTexture) {
	shader->setUniform(fragUnis::accum, accumulateTexture);
	shader->setUniform(fragUnis::reveal, revealTexture);
}
