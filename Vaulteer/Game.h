#pragma once

#include <iostream>
#include <unordered_map>

#include <finders_interface.h>

#include "Window.h"
#include "Camera.h"
#include "Event.h"
#include "Model.h"
#include "Scene.h"


#include "GraphicsData.h"

#include "DebugLogger.h"
#include "DebugAliases.h"

#include "Renderer.h"
#include "DeferredRenderer.h"
#include "ShaderProgram.h"
#include "ForwardTechnique.h"

class Game : public DebugLogger<Game>
{
public:
	Game(Window& window);
	void loadResources();
	size_t run();
	void setWindow(Window& window);
	/*
	template<class Data>
	using loadModel = std::make_pair<std::string, std::unique_ptr<Data>>;
	*/
private:
	template<class T>
	Model<T> modelByName(std::unordered_map<std::string, std::unique_ptr<T>>& container, std::string name);

	Window* window;
	std::unordered_map<std::string, std::unique_ptr<ModelData>> models;
	std::unordered_map<std::string, std::unique_ptr<LineData>> lines;

};

template<class T>
Model<T> Game::modelByName(std::unordered_map<std::string, std::unique_ptr<T>>& container, std::string name)
{
	auto it = container.find(name);
	if (it == container.end()) {
		debug("Model name not found.", MessageAlias::CriticalError);
	}
	else {
		return Model<T>(*it->second);
	}
}