#include "vpch.h"
#include "Application.h"

#include "Renderer/Renderer.h"


Application* Application::instance = nullptr;


Application::Application(const ApplicationSpecification& spec) : specification(spec) {
	assert(!instance); // "Application already exists, but construction was attempted."
	instance = this;

	if (!specification.workingDir.empty())
		std::filesystem::current_path(specification.workingDir);
}

void Application::init() {
	int success = glfwInit();
	assert(success); // "GLFW could not be initialized!"

	initWindow();

	OpenGL::initialize();

	Event::initialize();
	Event::setEventCallback(FORWARD_FN(onEvent));

	TextureLibrary::initDefaultTexture();

	isRunning = true;
	isMinimized = false;

	// Do any implementation specific setup here
	setup();
}

void Application::initWindow() {
	auto* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	int windowWidth = 1280, windowHeight = 720;
	int windowX = mode->width / 2 - windowWidth / 2;
	int windowY = mode->height / 2 - windowHeight / 2;

	WindowSpecification windowSpec = {specification.title, windowWidth, windowHeight, windowX, windowY};
	window = std::make_unique<Window>(windowSpec);

	auto rebuildGBufferFn = [&](int w, int h) {
		DeferredRenderer::resizeFramebuffers(w, h);
		BlendingForwardRenderer::resizeFramebuffers(w, h);
		TextRenderer::buildScreenProjection(w, h);
	};
	window->addResizeCallback(rebuildGBufferFn);
}

void Application::destroy() {
	window = nullptr;
	glfwTerminate();
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
	// TODO doesn't work because static objects don't get reset
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
	if (altHeld && e.checkRelease(KeyboardKey::ENTER)) {
		Event::window_fullscreen_callback(Window::getRawWindow(), !Window::fullscreen);
		return true;
	}

	return false;
}
