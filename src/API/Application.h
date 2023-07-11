#pragma once

#include <filesystem>
#include <ranges>

#include "Window.h"
#include "OpenGL.h"
#include "Model/Resources/ResourceManager.h"
#include "LayerStack.h"

#include "Events/Event.h"
#include "Model/Textures/Texture2D.h"

#include "Events/Event.h"
#include "Renderer/BlendingForwardRenderer.h"
#include "Renderer/DeferredRenderer.h"
#include "Renderer/TextRenderer.h"


struct ApplicationSpecification {
	std::string title = "Engine window";
	std::string workingDir; // empty string means lookup
};

class Application {
public:
	/* Constructor - only runs once */
	Application(const ApplicationSpecification& spec);
	virtual ~Application() = default;

	static Window& getWindow() { return *(instance->window); }

	/* Initialization - runs once on start/restart, calls subclass */
	void init();
	void initWindow();

	void destroy();

	/* Start game loop, calls update on every layer from back to front */
	size_t run();

	void onEvent(BaseEvent& e);


	virtual void setup() = 0;

	const ApplicationSpecification& specification;
	std::unique_ptr<Window> window;

	LayerStack layerStack;
	std::vector<std::unique_ptr<Layer>> layers;

	bool isRunning;
	bool isMinimized;

	float lastFrameTime = 0.0f;
	uint frameCounter = 0;

	static Application* instance;

private:
	bool onKeyboardButtonEvent(const KeyboardButtonEvent& e);
};
