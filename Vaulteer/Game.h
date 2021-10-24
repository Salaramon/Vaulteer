#pragma once

#include <iostream>
#include <unordered_map>

#include "Window.h"
#include "Camera.h"
#include "Event.h"
#include "Model.h"


#include "GraphicsData.h"

#include "DebugLogger.h"
#include "DebugAliases.h"

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
	Model modelByName(std::string name);

	Window* window;
	std::unordered_map<std::string, std::unique_ptr<GraphicsData>> models;

};

