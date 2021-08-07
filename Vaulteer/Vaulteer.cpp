
#define STB_IMAGE_IMPLEMENTATION

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <future>
#include <string>

#include "Window.h"
#include "Model.h"
#include "Camera.h"
#include "Event.h"

#include "GLSLCPPBinder.h"

//#include "VertexBuffer.h"
//#include "VertexArray.h"

#include "Shader.h"

#include "DebugLogger.h"
#include "DebugAliases.h"

void APIENTRY debugCallback(GLenum source​, GLenum type​, GLuint id​,
	GLenum severity​, GLsizei length​, const GLchar* message​, const void* userParam​) {
	DebugLogger<FUNCTION> log(ObjectAlias::OpenGLMessage);
	log.debug(std::string(message​) + '\n');
}

int main() {
	DebugLogger<>::disableLogging();
	DebugLogger<>::breakOnMessageName(MessageAlias::CriticalError);

	DebugLogger<FUNCTION> log(ObjectAlias::Main);
	
	int init = glfwInit();
	log.debug("GLFW initialization: " + std::to_string(init) + "\n", "glfwInit");

	

	Window window("Vaulteer", 1280, 720);

	glEnable(GL_DEBUG_OUTPUT);
	log.debug("GL_DEBUG_OUTPUT enabled.\n", "glEnable");

	glDebugMessageCallback(debugCallback, nullptr);
	log.debug("OpenGL debug callback set.\n", "glDebugMessageCallback");

	stbi_set_flip_vertically_on_load(true);
	log.debug("STBI vertical flip on load enabled.\n", "stbi_set_flip_vertically_on_load");

	glEnable(GL_DEPTH_TEST);
	log.debug("Depth test enabled.\n");

	Event::AddEventHandlingForWindow(&window);

	Shader shader(Binder::file_names::vertex, Binder::file_names::fragment);
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------

	Camera camera(1000, 45, (float)window.getWidth() / (float)window.getHeight(), Binder::vertex::uniforms::view, Binder::vertex::uniforms::projection);
	camera.setPosition(0, 0, -1);
	camera.setShaderContext(&shader);

	Texture::uniformTextureTypes.emplace(aiTextureType_DIFFUSE, Binder::fragment::uniforms::diffuse1);
	size_t modelObjs = 100;
	Model model("backpack/backpack.obj", "backpack", modelObjs);
	model.setShaderContext(&shader);

	for (size_t i = 0; i < modelObjs; i++) {
		model[i].setScale(1, 1, 1);
	}

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (window.is_running())
	{

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		log.debug("Color buffer clear color set.\n", "glClearColor");

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		log.debug("Color buffer bit and depth bit cleared.\n", "glClear");

		// draw our first triangle
		shader.use();
		camera.setPosition(0, 0, -1);
		camera.updateShaderMatrices();
		
		for (size_t i = 0; i < modelObjs; i++) {
			model[i].setPosition(20 * cos((glfwGetTime() / 100) * i), 20 * sin((glfwGetTime()/100) * i), -50);
		}
		/*
		model.setRotation(0.5 * glfwGetTime(), { 1,0,0 });
		model.setScale(1 + sin(glfwGetTime()) * 0.1, 1 + sin(glfwGetTime()) * 0.1, 1 + sin(glfwGetTime()) * 0.1);
		model.draw();
		
		model.setRotation(2.5 * glfwGetTime(), { 0, 1, 0 });
		model.setScale(1 + sin(glfwGetTime()) * 0.1, 1 + sin(glfwGetTime()) * 0.1, 1 + sin(glfwGetTime()) * 0.1);
		*/
		model.draw();

		Event::Poll();

		glfwSwapBuffers(window.getRawWindow());

		DebugLogger<>::disableLogging();
	}

	DebugLogger<>::setDefaultClassAccessLimit(8);
	//Not working??
	DebugLogger<>::printOrderedByMessage();

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}


//TODO
/*
Import and modify TileMap
Event handling

Make camera and make it able to rotate around the y axis
*/

