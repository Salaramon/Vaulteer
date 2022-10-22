#include "vpch.h"
#include "BlendingForwardRenderer.h"

void BlendingForwardRenderer::initialize(uint screenWidth, uint screenHeight) {
	alphaBuffer = std::make_unique<AlphaBuffer>(screenWidth, screenHeight);

	quad = ResourceLoader::importModel("resources/quad.obj");
}

void BlendingForwardRenderer::rebuildAlphaBuffer(int width, int height) {
	alphaBuffer.reset();
	alphaBuffer = std::make_unique<AlphaBuffer>(width, height);
}

