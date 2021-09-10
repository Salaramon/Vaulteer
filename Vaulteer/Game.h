#pragma once

#include <iostream>

#include "Camera.h"
#include "Event.h"
#include "Model.h"
class Game
{
public:
	void loadResources();
	void run();
private:
	std::vector<Model> models;

};

