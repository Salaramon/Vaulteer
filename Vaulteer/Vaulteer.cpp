
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>

#include "OpenGL.h"
#include "Event.h"
#include "Game.h"
#include "ShaderProgram.h"

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
	OpenGL::initialize();
	OpenGL::enableDebugOutput(OpenGL::SYNC);
	OpenGL::enableDepthTest();
	OpenGL::enableDirectDebugMessageing();
}

void initializeShaders() {
	ShaderProgram<ForwardShader>::load();
	ShaderProgram<LineShader>::load();
	ShaderProgram<DeferredShader>::load();
}

void cleanup() {
	glfwTerminate();
}

int main() {
	//Logger settings
	DebugLogger<>::disableLogging();
	//DebugLogger<>::breakAtMessage(206180);
	DebugLogger<>::breakOnMessageName(MessageAlias::CriticalError);
	
	//Initialization
	initializeGLFW();

	Window window("Vaulteer", 1280, 720);

	initializeOpenGL();
	initializeSTBI();

	initializeShaders();

	Event::AddEventHandlingForWindow(&window);
	Texture::uniformTextureTypes.emplace(aiTextureType_DIFFUSE, Binder::forward_frag::uniforms::diffuse1);
	//Texture::uniformTextureTypes.emplace(aiTextureType_SPECULAR, Binder::lightsource_frag::uniforms::lightColor);
	//Game game("resourceFolder?");

	Game game(window);
	size_t gameFlags = 1;

	while (gameFlags) {
		game.setWindow(window);
		game.loadResources();
		gameFlags = game.run();
	}

	

	cleanup();
	return 0;
}