#pragma once

#include "WorldLayer.h"
#include "API/Layer.h"

#include "Renderer/BlendingForwardRenderer.h"
#include "Renderer/DeferredRenderer.h"

class DebugLayer : public Layer {
public:
	DebugLayer(WorldLayer* world) : Layer("DebugLayer"), world(world) {}

	WorldLayer* world;

	//void onAttach() override;
	//void onUpdate(float timestep) override;

	void onEvent(BaseEvent& e) override {
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<KeyboardButtonEvent>(FORWARD_FN(onKeyboardButtonEvent));
	}

private:
	bool onKeyboardButtonEvent(const KeyboardButtonEvent& e) {
		if (e.checkPress(KeyboardKey::Q)) {
			DeferredRenderer::drawShadowVolumes = !DeferredRenderer::drawShadowVolumes;
			Log::info("Shadow volume visualization: {}", (DeferredRenderer::drawShadowVolumes ? "on" : "off"));
		}

		if (e.checkPress(KeyboardKey::T)) {
			world->renderer.reloadShaders();
			Log::info("Shaders reloaded");
		}
		// events should be allowed to bleed through debug layer
		return false;
	}
};
