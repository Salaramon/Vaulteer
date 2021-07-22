
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
#include "GeometryTechnique.h"



void handleCamera(MyCamera& camera) {
	float sens = 0.155f;
	float speed = 20.0f;

	Event::CursorEvents events = (Event::CursorEventList << (Event::CursorX > -9999999 && Event::CursorX < 9999999 && Event::CursorY > -9999999 && Event::CursorY < 9999999));
	if (!events.empty()) {
		double_t xDifference = events.back().CursorX - events.front().CursorX;
		double_t yDifference = events.back().CursorY - events.front().CursorY;

		camera.rotate(xDifference * sens / 1.35, yDifference * -sens);
	}

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

	GBuffer gbuffer = GBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

	Model model("teapot.obj");
	Model cube("cube.obj");
	Model quad("quad.obj");


	MyCamera camera(glm::vec3(.0f, 3.0f, -3.f), glm::vec3(.0f, .0f, 1.0f), glm::vec3(.0f, 1.0f, .0f), glm::vec3(.0f, .5f, 1.0f));
	MyCamera spotlight(glm::vec3(0.0f, 1.0f, -10.0f), glm::vec3(.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, .0f));

	ShadowMapFBO shadowMap = ShadowMapFBO();
	shadowMap.init(WINDOW_WIDTH, WINDOW_HEIGHT);

	GeometryTechnique geometryTech("geometry_vertex.shader", "geometry_frag.shader");
	LightingTechnique lightingTech("deferred_vertex.shader", "deferred_frag.shader");
	ShadowTechnique shadowTech("shadow_vertex.shader", "shadow_frag.shader");


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FRAMEBUFFER_SRGB);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);


	float deltaTime = 0, lastFrame = 0, startTime = glfwGetTime();
	std::cout << "Loaded in " << startTime << " seconds." << std::endl;
	
	while (window.is_running()) {

		lastFrame = deltaTime;
		deltaTime = glfwGetTime() - startTime;

		glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// geometry pass
        // -----------------------------------------------------------------------------------------------------------------------

		gbuffer.bindForWriting();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		geometryTech.use();

		geometryTech.setView(camera.getViewMatrix());
		geometryTech.setProjection(camera.getProjectionMatrix(WINDOW_WIDTH, WINDOW_HEIGHT));

		glm::mat4 origModelMat(1.0f);
		glm::mat4 modelMat = glm::translate(origModelMat, glm::vec3(0 * 3, 2.0f, 0 * 3));
		//modelMat = glm::rotate(modelMat, (float)glfwGetTime() * 240.f, glm::vec3(0.0f, 1.0f, 0.0f));

		geometryTech.setModel(modelMat);
		model.draw();

		geometryTech.setModel(glm::scale(origModelMat, glm::vec3(100.0f, 1.0f, 100.0f)));
		cube.draw();

		// unbind
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// light pass
        // -----------------------------------------------------------------------------------------------------------------------

		glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		lightingTech.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gbuffer.textures[GBuffer::Position]);
		glActiveTexture(GL_TEXTURE1);
 		glBindTexture(GL_TEXTURE_2D, gbuffer.textures[GBuffer::Normal]);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gbuffer.textures[GBuffer::Color]);

		glm::vec3 lightPosition(sinf(glfwGetTime() * 3) * 4, 6.0f, cosf(glfwGetTime() * 3) * 4);

		PointLight::Attenuation att = { 1.0f , 0.09f, 0.032f }; // for no dropoff: { 1.0f, .0f, .0f }
		PointLight pLight = { glm::vec3(1.0f, 1.0f, 1.0f), 0.01f, 0.2f, lightPosition, att };
		lightingTech.setPointLight(pLight);

		lightingTech.setWorldCameraPos(camera.position);

		lightingTech.setMaterialSpecularIntensity(1.0f);
		lightingTech.setMaterialShininess(8.0f);

		/*

		lightingTech.setView(camera.getViewMatrix());
		lightingTech.setProjection(camera.getProjectionMatrix(WINDOW_WIDTH, WINDOW_HEIGHT));


		PointLight::Attenuation att = { 1.0f , 0.09f, 0.032f }; // for no dropoff: { 1.0f, .0f, .0f }
		PointLight::Attenuation noAtt = { 1.0f , 0.0f, 0.0f }; // for no dropoff: { 1.0f, .0f, .0f }
		//PointLight pLight = { glm::vec3(1.0f, 1.0f, 1.0f), 0.05f, 1.0f, glm::vec3(0, 1.0f, 0), att };
		PointLight pLight = { glm::vec3(1.0f, 1.0f, 1.0f), 0.05f, 1.0f, glm::vec3(sinf(glfwGetTime() * 3) * 4, 10.0f, cosf(glfwGetTime() * 3) * 4), att };

		SpotLight light = { glm::vec3(1.0f), 0.05f, 1.0f, camera.position, noAtt, camera.front, 0.96f };

		lightingTech.setPointLight(pLight);
		//lightingTech.setSpotLight(light);

		lightingTech.setWorldCameraPos(camera.position);

		lightingTech.setMaterialSpecularIntensity(1.0f);
		lightingTech.setMaterialShininess(8.0f);

		glm::mat4 origModelMat(1.0f);
		glm::mat4 modelMat = glm::translate(origModelMat, glm::vec3(0 * 3, 3.0f, 0 * 3));
		//modelMat = glm::rotate(modelMat, (float)glfwGetTime() * 240.f, glm::vec3(0.0f, 1.0f, 0.0f));

		lightingTech.setModel(modelMat);
		//shader.setUniform("shadowMap", 0); 

		model.draw();

		modelMat = glm::scale(origModelMat, glm::vec3(100.0f, 1.0f, 100.0f));
		lightingTech.setModel(modelMat);
		cube.draw();*/

		// render quad to default framebuffer
		quad.draw();

		// 2.5. copy content of geometry's depth buffer to default framebuffer's depth buffer
		// ----------------------------------------------------------------------------------
		gbuffer.bindForReading();
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
		// blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
		// the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the 		
		// depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
		glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

