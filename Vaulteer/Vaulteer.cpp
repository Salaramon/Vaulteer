
#define STB_IMAGE_IMPLEMENTATION

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <future>

#include "Window.h"
#include "Shader.h"
#include "Model.h"
#include "MyCamera.h"
#include "Camera.h"
#include "ShaderCode.h"
#include "ShadowMapFBO.h"

#include "Event.h"
#include "GBuffer.h"


#include "GLSLCPPBinder.h"


void handleCamera(MyCamera& camera) {

	Event::CursorEvents events = (Event::CursorEventList << (Event::CursorX > -9999999 && Event::CursorX < 9999999 && Event::CursorY > -9999999 && Event::CursorY < 9999999));
	if (!events.empty()) {
		double_t xDifference = events.back().CursorX - events.front().CursorX;
		double_t yDifference = events.back().CursorY - events.front().CursorY;

		camera.rotate(xDifference * 0.1f, yDifference * -0.1f);
	}

	float speed = 5.0f;
	if (Event::Check << (Event::Key == Event::KEY::W && Event::State == Event::STATE::DOWN)) {
		camera.move(camera.front * (float)Event::delta() * speed);
	}
	if (Event::Check << (Event::Key == Event::KEY::S && Event::State == Event::STATE::DOWN)) {
		camera.move(-camera.front * (float)Event::delta() * speed);
	}
	if (Event::Check << (Event::Key == Event::KEY::A && Event::State == Event::STATE::DOWN)) {
		camera.move(-camera.right * (float)Event::delta() * speed);
	}
	if (Event::Check << (Event::Key == Event::KEY::D && Event::State == Event::STATE::DOWN)) {
		camera.move(camera.right * (float)Event::delta() * speed);
	}
}

int main() {
	glfwInit();

	const int WINDOW_WIDTH = 1280, WINDOW_HEIGHT = 720;

	Window window("Vaulteer", WINDOW_WIDTH, WINDOW_HEIGHT);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Event::AddEventHandlingForWindow(&window);
	glfwSetInputMode(window.getRawWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	Shader shader = Shader("standard_vertex.shader", "standard_frag.shader");
	shader.use();

	Model model("teapot.obj");
	Model quad("quad.obj");


	MyCamera camera = MyCamera(glm::vec3(.0f, .0f, -5.f), glm::vec3(.0f, .0f, 1.0f), glm::vec3(.0f, 1.0f, .0f), glm::vec3(.0f, .0f, 1.0f));
	MyCamera spotlight = MyCamera(glm::vec3(0.0f, 1.0f, -10.0f), glm::vec3(.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, .0f));

	ShadowMapFBO shadowMap = ShadowMapFBO();
	shadowMap.init(WINDOW_WIDTH, WINDOW_HEIGHT);



	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	float deltaTime = 0, lastFrame = 0;
	
	while (window.is_running()) {

		// shadow map pass

		shadowMap.bindWrite();
		
		glClear(GL_DEPTH_BUFFER_BIT);

		glm::mat4 sh_model(1.0f);

		shader.setMatrix("model", sh_model);
		shader.setMatrix("view", spotlight.getViewMatrix());
		shader.setMatrix("projection", spotlight.getProjectionMatrix(1.0f, 1.0f));

		model.draw(shader);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		shader.use();

		// render pass

		shadowMap.bindRead(GL_TEXTURE0);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 modelMat(1.0f);
		//modelMat = glm::translate(modelMat, glm::vec3(0.0f, -1.0f, 0.0f));
		//modelMat = glm::rotate(modelMat, (float) (glfwGetTime() * .2f), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = camera.getProjectionMatrix(WINDOW_WIDTH, WINDOW_HEIGHT);

		shader.setMatrix("model", modelMat);
		shader.setMatrix("view", view);
		shader.setMatrix("projection", projection);

		shader.setUniform("shadowMap", 0);

		quad.draw(shader);

		glActiveTexture(0);

		glfwSwapBuffers(window.getRawWindow());

		Event::Poll();
		handleCamera(spotlight);
	}

	glfwTerminate();
	return 0;
}


//TODO
/*
Import and modify TileMap
Event handling

Make camera and make it able to rotate around the y axis
*/

