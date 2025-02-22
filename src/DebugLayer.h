#pragma once

#include "WorldLayer.h"
#include "API/Layer.h"

#include "Renderer/BlendingForwardRenderer.h"
#include "Renderer/DebugRenderer.h"
#include "Renderer/DeferredRenderer.h"

namespace DebugLayer {

	World* world;
	inline Renderer<DebugRenderer, TextRenderer> renderer;

	bool inspectorEnabled = false;
	struct InspectorStats {
		int materialID = -1;
		int instanceID = -1;
	} inspector;

	void onAttach(void* context) {
		DebugRenderer::initialize(Window::getWidth(), Window::getHeight());
	}

	void onUpdate(void* context, float timestep) {
		if (inspectorEnabled) {
			std::string stats = inspector.materialID != -1 && inspector.instanceID != -1 ?
				std::format("Material ID: {}\nInstance ID: {}", inspector.materialID, inspector.instanceID) :
				"Empty";
			TextRenderer::submitText(stats, TextRenderer::screenMiddle + glm::vec2(30, 30), 0.5f);
			
			renderer.render(world->scene);

			TextRenderer::clearScene();
		}
	}
	
	bool onKeyboardButtonEvent(const KeyboardButtonEvent& e);
	bool onMouseButtonEvent(const MouseButtonEvent& e);

	void onEvent(void* context, BaseEvent& e) {
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<KeyboardButtonEvent>(FORWARD_FN(DebugLayer::onKeyboardButtonEvent));
		dispatcher.dispatch<MouseButtonEvent>(FORWARD_FN(DebugLayer::onMouseButtonEvent));
	}


	// -- event handling implementations --

	bool onKeyboardButtonEvent(const KeyboardButtonEvent& e) {
		if (e.checkPress(KeyboardKey::Q)) {
			DeferredRenderer::drawShadowVolumes = !DeferredRenderer::drawShadowVolumes;
			Log::info("Shadow volume visualization: {}", (DeferredRenderer::drawShadowVolumes ? "on" : "off"));
		}

		if (e.checkPress(KeyboardKey::T)) {
			WorldLayer::renderer.reloadShaders();
			Log::info("Shaders reloaded");
		}

		if (e.checkPress(KeyboardKey::F2)) {
			inspectorEnabled = !inspectorEnabled;
		}
		// events should be allowed to bleed through debug layer
		return false;
	}

	bool onMouseButtonEvent(const MouseButtonEvent& e) {
		if (e.checkPress(MouseKey::MOUSE_LEFT)) {
			DebugRenderer::inspectorBuffer->bindForReading();

			// just get screen middle and post the pixel
			glm::ivec2 mousePos;
			glfwGetWindowSize(Window::getRawWindow(), &mousePos.x, &mousePos.y);
			mousePos /= 2;

			inspector.materialID = DebugRenderer::inspectorBuffer->getPixel(0, mousePos.x, mousePos.y);
			inspector.instanceID = DebugRenderer::inspectorBuffer->getPixel(1, mousePos.x, mousePos.y);

			Framebuffer::unbind();
		}
		return false;
	}

	LayerFunctions debugFunctions {
		.context = &WorldLayer::world,
		.onAttach = &onAttach,
		.onEvent = &onEvent,
		.onUpdate = &onUpdate
	};

};
