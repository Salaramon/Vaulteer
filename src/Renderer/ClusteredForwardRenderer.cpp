#include "vpch.h"
#include "ClusteredForwardRenderer.h"

ClusteredForwardRenderer::ClusteredForwardRenderer() : computedTilingTex(1280, 720) {
	computedTilingTex.setMinifyingFilter(GL_NEAREST);
	computedTilingTex.setMagnifyingFilter(GL_NEAREST);
	glBindTextureUnit(0, computedTilingTex.textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, work_size.x, work_size.y, 0, GL_RED, GL_FLOAT, NULL);
	glBindImageTexture(0, computedTilingTex.textureID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);
}

/*void ClusteredForwardRenderer::render(Scene& scene) {
	ClusterTileTechnique::shader->use();
	glBindTextureUnit(0, computedTilingTex.textureID);

	float values[12] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, work_size.x, work_size.y, 0, GL_RED, GL_FLOAT, values);

	ClusterTileTechnique::shader->use();
	glBindTextureUnit(0, computedTilingTex.textureID);

	glDispatchCompute(work_size.x, work_size.y, 1);

	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	auto collection_size = work_size.x * work_size.y;
	std::vector<float> compute_data(collection_size);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, compute_data.data());

	for (auto d : compute_data) {
		std::cout << d << " ";
	}
	std::cout << std::endl;

}*/
