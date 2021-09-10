
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>

#include "OpenGL.h"
#include "Event.h"
#include "Game.h"

#include "DebugLogger.h"
#include "DebugAliases.h"


void initializeSTBI() {
	DebugLogger<FUNCTION> log(ObjectAlias::STBI);
	stbi_set_flip_vertically_on_load(true);
	log.debug("Vertical flip on load enabled.\n", "stbi_set_flip_vertically_on_load()");
}

void initializeGLFW() {
	DebugLogger<FUNCTION> log(ObjectAlias::GLFW);
	int init = glfwInit();
	log.debug("Initialization: " + std::to_string(init) + "\n", "glfwInit()");
}

void initializeOpenGL() {
	OpenGL::enableDebugOutput(OpenGL::SYNC);
	OpenGL::enableDepthTest();
	OpenGL::enableDirectDebugMessageing();
}

void cleanup() {
	glfwTerminate();
}

int main() {
	//Logger settings
	DebugLogger<>::disableLogging();
	DebugLogger<>::breakOnMessageName(MessageAlias::CriticalError);
	
	//Initialization
	initializeGLFW();

	Window window("Vaulteer", 1280, 720);

	initializeOpenGL();
	initializeSTBI();


	Event::AddEventHandlingForWindow(&window);

	//Game game("resourceFolder?");


	Game game;
	size_t count = 0;
	double startTime = glfwGetTime();
	while (window.is_running()) {
		game.run();
	}

	

	cleanup();
	return 0;
}