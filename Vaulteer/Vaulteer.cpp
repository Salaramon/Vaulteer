
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
#include "LightingTechnique.h"
#include "ShadowTechnique.h"
#include "LightTypes.h"

#include "Event.h"
#include "GBuffer.h"



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


	/*Shader shader = Shader("standard_vertex.shader", "standard_frag.shader");
	shader.use();*/

	Model model("teapot.obj");
	Model quad("quad.obj");


	MyCamera camera(glm::vec3(.0f, .0f, -3.f), glm::vec3(.0f, .0f, 1.0f), glm::vec3(.0f, 1.0f, .0f), glm::vec3(.0f, .0f, 1.0f));
	MyCamera spotlight(glm::vec3(0.0f, 1.0f, -10.0f), glm::vec3(.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, .0f));

	ShadowMapFBO shadowMap = ShadowMapFBO();
	shadowMap.init(WINDOW_WIDTH, WINDOW_HEIGHT);

	LightingTechnique lightingTech("light_vertex.shader", "light_frag.shader");
	ShadowTechnique shadowTech("standard_vertex.shader", "standard_frag.shader");


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	float deltaTime = 0, lastFrame = 0, startTime = glfwGetTime();
	std::cout << "Loaded in " << startTime << " seconds." << std::endl;
	
	while (window.is_running()) {

		lastFrame = deltaTime;
		deltaTime = glfwGetTime() - startTime;

		// shadow map pass

		/*shadowMap.bindWrite();
		shadowTech.use();
		
		glClear(GL_DEPTH_BUFFER_BIT);

		glm::mat4 sh_model(1.0f);

		shadowTech.setModel(sh_model);
		shadowTech.setView(spotlight.getViewMatrix());
		shadowTech.setProjection(spotlight.getProjectionMatrix(WINDOW_WIDTH, WINDOW_HEIGHT));

		model.draw();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		*/

		// render pass

		//shadowMap.bindRead(GL_TEXTURE0);

		glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingTech.use();

		glm::mat4 modelMat(1.0f);
		modelMat = glm::translate(modelMat, glm::vec3(0.0f, -1.0f, 0.0f));
		//modelMat = glm::scale(modelMat, glm::vec3((float) (WINDOW_WIDTH / (float)WINDOW_HEIGHT), 1.0f, 1.0f));
		modelMat = glm::rotate(modelMat, (float)glfwGetTime() * 0.5f, glm::vec3(.5f, 1.0f, 0.0f));

		lightingTech.setModel(modelMat);
		lightingTech.setView(camera.getViewMatrix());
		lightingTech.setProjection(camera.getProjectionMatrix(WINDOW_WIDTH, WINDOW_HEIGHT));

		float intensity = sinf(glfwGetTime()) / 2 + 0.5;

		PointLight::Attenuation att = { 1.0f , 0.09f, 0.032f };
		PointLight light = { glm::vec3(1.0f), 0.05f, 1.0f, glm::vec3(.0f, sinf(glfwGetTime())*2 + 6, .0f), att };

		glm::vec3 dir = { cosf(glfwGetTime() * 1), 0.0f, sinf(glfwGetTime() * 1) };

		lightingTech.setPointLight(light);

		lightingTech.setWorldCameraPos(camera.position);

		lightingTech.setMaterialSpecularIntensity(1.0f);
		lightingTech.setMaterialSpecularPower(16.0f);

		//shader.setUniform("shadowMap", 0);

		model.draw();

		glfwSwapBuffers(window.getRawWindow());

		Event::Poll();
		handleCamera(camera);
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

