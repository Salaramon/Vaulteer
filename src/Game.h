#pragma once

#include <iostream>
#include <unordered_map>
#include <functional>

#include "API/Window.h"
#include "API/Camera.h"
#include "API/Event.h"

#include "Data Structures/BoundingSphereHierarchy.h"

#include "Model/Model.h"
#include "Model/Resources/ResourceManager.h"

#include "Scene/DynamicScene.h"
#include "Scene/StaticScene.h"

#include "Renderer/Renderer.h"
#include "Renderer/BlendingForwardRenderer.h"
#include "Renderer/ShaderProgram.h"

#include "Opaque.h"
#include "Transparent.h"

class Game
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

	ResourceManager resourceManager;

	std::unordered_map<std::string, std::unique_ptr<ModelData>> models;
	std::unordered_map<std::string, std::unique_ptr<LineData>> lines;

};

template<class T>
Model<T> Game::modelByName(std::unordered_map<std::string, std::unique_ptr<T>>& container, std::string name)
{
	auto it = container.find(name);
	if (it == container.end()) {
		//debug("Model name not found.", MessageAlias::CriticalError);
	}
	else {
		return Model<T>(*it->second);
	}
}