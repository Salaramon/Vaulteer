
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

float randf(float LO, float HI) {
	return LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
}

glm::vec3 getColor(float offset) {
	float diff0 = fmax(1 - fmin(offset, 3 - offset), 0.0f);
	float diff1 = fmax(1 - fabsf(1 - offset), 0.0f);
	float diff2 = fmax(1 - fabsf(2 - offset), 0.0f);
	return glm::vec3(diff0, diff1, diff2);
}

int main() {
	glfwInit();

	const int WINDOW_WIDTH = 1280, WINDOW_HEIGHT = 720;

	Window window("Vaulteer", WINDOW_WIDTH, WINDOW_HEIGHT);

	Event::AddEventHandlingForWindow(&window);
	glfwSetInputMode(window.getRawWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	/*Shader shader = Shader("standard_vertex.glsl", "standard_frag.glsl");
	shader.use();*/

	GBuffer gbuffer = GBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

	Model model("teapot.obj");
	Model cube("cube.obj");
	Model quad("quad.obj");

	unsigned int nullTextureID;
	glGenTextures(1, &nullTextureID);
	glBindTexture(GL_TEXTURE_2D, nullTextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	MyCamera camera(glm::vec3(.0f, 3.0f, -3.f), glm::vec3(.0f, .0f, 1.0f), glm::vec3(.0f, 1.0f, .0f), glm::vec3(.0f, .5f, 1.0f));
	MyCamera spotlight(glm::vec3(0.0f, 1.0f, -10.0f), glm::vec3(.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, .0f));

	ShadowMapFBO shadowMap = ShadowMapFBO();
	shadowMap.init(WINDOW_WIDTH, WINDOW_HEIGHT);

	GeometryTechnique geometryTech("geometry_vertex.glsl", "geometry_frag.glsl");
	LightingTechnique lightingTech("deferred_vertex.glsl", "deferred_frag.glsl");
	ShadowTechnique shadowTech("shadow_vertex.glsl", "shadow_frag.glsl");
	Shader lightSourceShader = Shader("lightsource_vertex.glsl", "lightsource_frag.glsl");


	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_FRAMEBUFFER_SRGB);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	const int NUM_LIGHTS = 32;
	glm::vec3 lightPositions[NUM_LIGHTS];
	float lightColorOffsets[NUM_LIGHTS];
	int8_t lightDirs[NUM_LIGHTS];
	PointLight pointLights[NUM_LIGHTS];
	PointLight::Attenuation att = { 1.0f, 0.19f, 0.132f }; // for no dropoff: { 1.0f, .0f, .0f }

	srand((int) glfwGetTime());
	for (int i = 0; i < NUM_LIGHTS; i++) {
		lightDirs[i] = (rand() % 2) * 2 - 1;
		lightPositions[i] = glm::vec3(0.0, randf(1.0f, 3.0f), randf(-10.0f, 10.0f));
		lightColorOffsets[i] = randf(0.0f, 3.0f);
		pointLights[i] = { glm::vec3(randf(0.0f, 1.0f), randf(0.0f, 1.0f), randf(0.0f, 1.0f)), 0.01f, 0.2f, lightPositions[i], att };
	}


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

		geometryTech.use();

		geometryTech.setView(camera.getViewMatrix());
		geometryTech.setProjection(camera.getProjectionMatrix(WINDOW_WIDTH, WINDOW_HEIGHT));

		glm::mat4 origModelMat(1.0f);
		glm::mat4 modelMat = glm::translate(origModelMat, glm::vec3(0 * 3, 2.0f, 0 * 3));

		geometryTech.setTexture(nullTextureID);
		geometryTech.setModel(modelMat);
		model.draw();
		geometryTech.setModel(glm::scale(glm::translate(origModelMat, glm::vec3(0, 3.0f, 0)), glm::vec3(3.0f, 3.0f, 1.0f)));
		quad.draw();

		geometryTech.setModel(glm::scale(origModelMat, glm::vec3(100.0f, 1.0f, 100.0f)));
		cube.draw();

		// unbind
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// light pass
        // -----------------------------------------------------------------------------------------------------------------------

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingTech.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gbuffer.textures[GBuffer::Position]);
		glActiveTexture(GL_TEXTURE1);
 		glBindTexture(GL_TEXTURE_2D, gbuffer.textures[GBuffer::Normal]);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gbuffer.textures[GBuffer::Color]);

		glm::vec3 lightCurrentPos[NUM_LIGHTS];
		glm::vec3 lightCurrentColor[NUM_LIGHTS];
		for (int i = 0; i < NUM_LIGHTS; i++) {
			lightCurrentPos[i] = glm::vec3(
				lightDirs[i] * sinf(glfwGetTime() + lightPositions[i].y) * lightPositions[i].x,
				lightPositions[i].y, 
				lightDirs[i] * cosf(glfwGetTime() + lightPositions[i].y) * lightPositions[i].z);

			lightCurrentColor[i] = getColor(fmod(lightColorOffsets[i] + glfwGetTime() * lightColorOffsets[i], 3.0f));
		}

		for (int i = 0; i < NUM_LIGHTS; i++) {
			pointLights[i].position = lightCurrentPos[i];
			if (lightCurrentPos[i].z > 0.0)
				pointLights[i].color = lightCurrentColor[i];
			else
				pointLights[i].color = glm::vec3(0.0);
			lightingTech.setPointLight(pointLights[i], i);
		}

		lightingTech.setWorldCameraPos(camera.position);

		lightingTech.setMaterialSpecularIntensity(1.0f);
		lightingTech.setMaterialShininess(32.0f);

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

		// light source pass

		lightSourceShader.use();
		lightSourceShader.setMatrix("view", camera.getViewMatrix());
		lightSourceShader.setMatrix("projection", camera.getProjectionMatrix(WINDOW_WIDTH, WINDOW_HEIGHT));

		for (int i = 0; i < NUM_LIGHTS; i++) {
			glm::mat4 modelMat = glm::translate(origModelMat, lightCurrentPos[i] + glm::vec3(0, -0.05, 0));
			modelMat = glm::scale(modelMat, glm::vec3(0.1f));
			modelMat = glm::rotate(modelMat, (float)glfwGetTime() * 3, glm::vec3(0.0f, 1.0f, 0.0f));

			lightSourceShader.setMatrix("model", modelMat);
			lightSourceShader.setVector("lightColor",  pointLights[i].color);

			model.draw();
		}

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

