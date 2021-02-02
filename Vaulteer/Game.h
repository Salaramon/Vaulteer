#pragma once



#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Window.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"

#include "Event.h"

class Game
{
public:
	Game();
	void run();
	Window *window;
	bool done = false;
private:
	
	//Shader shader;
	float deltaTime = 0;
	float lastFrame = 0;
	GLFWwindow *mainContext;
};

