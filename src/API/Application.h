#pragma once

#include <filesystem>

#include "OpenGL.h"
#include "Model/Resources/ResourceManager.h"
#include "LayerStack.h"


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
	static Application& get() { return *instance; }

	/* Initialization - runs once on start/restart, calls subclass */
	void init();

	/* Start game loop, calls update on every layer from back to front */
	size_t run();
	
protected:
	virtual void setup() = 0;

	std::unique_ptr<Window> window;
	const ApplicationSpecification& specification;

	LayerStack layerStack;
	std::vector<std::unique_ptr<Layer>> layers;

	bool isRunning = true;
	bool isMinimized = false;

	float lastFrameTime = 0.0f;

private:
	static Application* instance;
};

