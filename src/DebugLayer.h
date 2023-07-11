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
	bool onKeyboardButtonEvent(KeyboardButtonEvent& e) {
		if (e.button.key == KeyboardKey::Q && e.button.action == KeyAction::PRESS) {
			DeferredRenderer::drawShadowVolumes = !DeferredRenderer::drawShadowVolumes;
			std::cout << "Toggled shadow volume visualization" << std::endl;
		}

		if (e.button.key == KeyboardKey::T && e.button.action == KeyAction::RELEASE) {
			world->renderer.reloadShaders();
			std::cout << "Shaders reloaded" << std::endl;
		}
		// events should be allowed to bleed through debug layer
		return false;
	}
};
