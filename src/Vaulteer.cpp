#include "vpch.h"

#define STB_IMAGE_IMPLEMENTATION

#include <iostream>

#include <DebugYourself.h>

#include "API/Event.h"
#include "Renderer/ShaderProgram.h"

#include "Game.h"



void initializeSTBI() {
	stbi_set_flip_vertically_on_load(true);
	//log.debug("Vertical flip on load enabled.\n", "stbi_set_flip_vertically_on_load()");
}

void initializeGLFW() {
	int init = glfwInit();
	//log.debug("Initialization: " + std::to_string(init) + "\n", "glfwInit()");
}

void initializeOpenGL() {
	OpenGL::initialize();
	//OpenGL::enableDebugOutput(OpenGL::SYNC);
	OpenGL::enableDepthTest();
	OpenGL::enableStencilTest();
	//OpenGL::enableSRGB();
	//OpenGL::enableDirectDebugMessageing();
}

void initializeShaders() {
	ShaderProgram<BlendingShader>::load();
	ShaderProgram<BlendingCompositeShader>::load();
	ShaderProgram<LineShader>::load();
	ShaderProgram<DeferredShader>::load();
	ShaderProgram<GeometryShader>::load();
	ShaderProgram<VolumeShader>::load();
}

void cleanup() {
	glfwTerminate();
}

int main() {
	Shader::CB.use(Shader::CB);
	Shader::UniformFunctor::FB.use(Shader::UniformFunctor::FB);

	//Initialization
	initializeGLFW();

	Window window("Vaulteer", 1280, 720);

	initializeOpenGL();
	initializeSTBI();

	initializeShaders();

	Event::AddEventHandlingForWindow(&window);
	//Texture::uniformTextureTypes.emplace(aiTextureType_DIFFUSE, Binder::forward_frag::uniforms::diffuse1);
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

	DY::end();
	return 0;
}
