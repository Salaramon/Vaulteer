#include "vpch.h"
#include "DebugLayer.h"

#include "Renderer/BlendingForwardRenderer.h"
#include "Renderer/DeferredRenderer.h"
#include "Renderer/Shader.h"


void DebugLayer::onEvent(BaseEvent& e) {

	EventDispatcher dispatcher(e);
	dispatcher.dispatch<KeyboardButtonEvent>(FORWARD_FN(onKeyboardButtonEvent));

}

bool DebugLayer::onKeyboardButtonEvent(KeyboardButtonEvent& e) {
	if (e.button.key == KeyboardKey::T) {
		DeferredRenderer::reloadShaders();
		BlendingForwardRenderer::reloadShaders();
		std::cout << "Shaders reloaded" << std::endl;
	}
	// events should be allowed to bleed through debug layer
	return false;
}
