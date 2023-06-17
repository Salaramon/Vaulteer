#include "vpch.h"
#include "Application.h"

#include "Renderer/Renderer.h"


Application* Application::instance = nullptr;


Application::Application(const ApplicationSpecification& spec) : specification(spec) {
	assert(!instance); // "Application already exists, but construction was attempted."
	instance = this;

	int success = glfwInit();
	assert(success); // "GLFW could not be initialized!"

	if (!spec.workingDir.empty())
		std::filesystem::current_path(spec.workingDir);

	auto* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	int windowWidth = 1280, windowHeight = 720;
	int windowX = mode->width / 2 - windowWidth / 2;
	int windowY = mode->height / 2 - windowHeight / 2;

	WindowSpecification windowSpec = {spec.title, windowWidth, windowHeight, windowX, windowY};
	window = std::make_unique<Window>(windowSpec);

	OpenGL::initialize();

	Event::initialize();
	Event::setEventCallback(FORWARD_FN(onEvent));

	TextureLibrary::initDefaultTexture();
}

void Application::init() {
	auto rebuildGBufferFn = [&](int w, int h) {
		DeferredRenderer::rebuildGBuffer(w, h);
		BlendingForwardRenderer::rebuildAlphaBuffer(w, h);
		TextRenderer::buildScreenProjection(w, h);
	};
	window->addResizeCallback(rebuildGBufferFn);

	// Do any implementation specific setup here
	setup();
}

size_t Application::run() {
	while (isRunning) {
		Event::poll();

		float time = glfwGetTime();
		float timestep = time - lastFrameTime;
		lastFrameTime = time;
		frameCounter++;

		for (Layer* layer : layerStack) {
			layer->onUpdate(timestep);
		}

		isRunning = window->onUpdate();
	}

	// to note: this method can return anything else to reinitalize window and game state
	return 0;
}



void Application::onEvent(BaseEvent& e) {

	EventDispatcher dispatcher(e);
	dispatcher.dispatch<KeyboardButtonEvent>(FORWARD_FN(onKeyboardButtonEvent));

	dispatcher.dispatch<WindowCloseEvent>(FORWARD_FN(window->onWindowCloseEvent));
	dispatcher.dispatch<WindowFullscreenEvent>(FORWARD_FN(window->onWindowFullscreenEvent));
	dispatcher.dispatch<WindowMaximizeEvent>(FORWARD_FN(window->onWindowMaximizeEvent));
	dispatcher.dispatch<WindowPositionEvent>(FORWARD_FN(window->onWindowPositionEvent));
	dispatcher.dispatch<WindowFocusEvent>(FORWARD_FN(window->onWindowFocusEvent));
	dispatcher.dispatch<WindowResizeEvent>(FORWARD_FN(window->onWindowResizeEvent));

	for (auto& it : std::ranges::reverse_view(layerStack)) {
		if (e.handled)
			return;

		it->onEvent(e);
	}
}

bool Application::onKeyboardButtonEvent(const KeyboardButtonEvent& e) {
	// toggle fullscreen state on alt+enter;
	bool altHeld = Event::state(KeyboardKey::LEFT_ALT) || Event::state(KeyboardKey::RIGHT_ALT);
	if (altHeld && e.button.key == KeyboardKey::ENTER && e.button.action == KeyAction::RELEASE) {
		Event::window_fullscreen_callback(Window::getRawWindow(), !Window::fullscreen);
		return true;
	}

	return false;
}
