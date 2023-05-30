#include "vpch.h"
#include "Application.h"

#include "Renderer/Renderer.h"


Application* Application::instance = nullptr;


Application::Application(const ApplicationSpecification& spec) : specification(spec) {
	assert(!instance); // "Application already exists, but construction was attempted."
	instance = this;

	if (!specification.workingDir.empty())
		std::filesystem::current_path(specification.workingDir);
	
	window = std::make_unique<Window>(specification.title, 1280, 720);

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
	dispatcher.dispatch<WindowCloseEvent>(FORWARD_FN(window->onWindowCloseEvent));
	dispatcher.dispatch<WindowFocusEvent>(FORWARD_FN(window->onWindowFocusEvent));
	dispatcher.dispatch<WindowResizeEvent>(FORWARD_FN(window->onWindowResizeEvent));

	for (auto& it : std::ranges::reverse_view(layerStack)) {
		if (e.handled)
			return;

		it->onEvent(e);
	}
}
