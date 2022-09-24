#include "vpch.h"
#include "BlendingForwardRenderer.h"

void BlendingForwardRenderer::initialize(uint screenWidth, uint screenHeight) {
	alphaBuffer = std::make_unique<AlphaBuffer>(screenWidth, screenHeight);

	quad = std::make_unique<ModelData>(std::move(ResourceLoader::importModel("resources/quad.obj")));
}

void BlendingForwardRenderer::rebuildAlphaBuffer(int width, int height) {
	alphaBuffer.reset();
	alphaBuffer = std::make_unique<AlphaBuffer>(width, height);
}

