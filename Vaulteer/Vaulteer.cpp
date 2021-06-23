
#define STB_IMAGE_IMPLEMENTATION

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <future>

#include "Window.h"
#include "Model.h"
#include "Camera.h"
#include "Event.h"

#include "GLSLCPPBinder.h"

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexAttribute.h"

#include "Shader.h"


void APIENTRY debugCallback(GLenum source​, GLenum type​, GLuint id​,
	GLenum severity​, GLsizei length​, const GLchar* message​, const void* userParam​) {
	std::cout << message​ << std::endl;
}

int main() {

	glfwInit();

	

	Window window("Vaulteer", 1280, 720);
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(debugCallback, nullptr);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	stbi_set_flip_vertically_on_load(true);
	glEnable(GL_DEPTH_TEST);

	Event::AddEventHandlingForWindow(&window);

	Shader shader("vertex.shader", "fragment.shader");
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------

	Camera camera(1000,45,window.getWidth()/window.getHeight(),Binder::vertex::uniforms::view, Binder::vertex::uniforms::projection);
	camera.setShaderContext(&shader);

	Texture::uniformTextureTypes.emplace(aiTextureType_DIFFUSE, Binder::fragment::uniforms::diffuse1);
	Model model("backpack/backpack.obj", "backpack");
	model.setShaderContext(&shader);
	

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (window.is_running())
	{

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw our first triangle
		shader.use();
		camera.updateShaderMatrices();

		model.setPosition(2*cos(glfwGetTime()*2) + 5, 2*sin(glfwGetTime()*2), -25);
		model.setRotation(0.5 * glfwGetTime(), {1,0,0});
		model.setScale(1 + sin(glfwGetTime()) * 0.1, 1 + sin(glfwGetTime()) * 0.1, 1 + sin(glfwGetTime()) * 0.1);
		model.draw();
		model.setPosition(2*cos(glfwGetTime()*2) - 5, 2*sin(glfwGetTime()*2), -25);
		model.setRotation(2.5 * glfwGetTime(), { 0, 1, 0 });
		model.setScale(1 + sin(glfwGetTime()) * 0.1, 1 + sin(glfwGetTime()) * 0.1, 1 + sin(glfwGetTime()) * 0.1);
		model.draw();

		Event::Poll();

		glfwSwapBuffers(window.getRawWindow());

	}


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

