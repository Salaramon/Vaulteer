
#define STB_IMAGE_IMPLEMENTATION

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <future>
#include <string>

#include "Window.h"
#include "Model.h"
#include "MyCamera.h"
#include "Camera.h"
#include "LightingTechnique.h"
#include "ShadowTechnique.h"
#include "ShadowCubeTechnique.h"
#include "LightTypes.h"

#include "Event.h"
#include "GBuffer.h"
#include "GeometryTechnique.h"

#include "Shader.h"

#include "DebugLogger.h"
#include "DebugAliases.h"

#include "TypeDefUtils.h"


void APIENTRY debugCallback(GLenum source​, GLenum type​, GLuint id​,
	GLenum severity​, GLsizei length​, const GLchar* message​, const void* userParam​) {
	DebugLogger<FUNCTION> log(ObjectAlias::OpenGLMessage);
	std::cout << std::string(message​) + '\n' << std::endl;
	log.debug(std::string(message​) + '\n');
}


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
	//DebugLogger<>::disableLogging();
	DebugLogger<>::breakOnMessageName(MessageAlias::CriticalError);

	DebugLogger<FUNCTION> log(ObjectAlias::Main);
	
	int init = glfwInit();
	log.debug("GLFW initialization: " + std::to_string(init) + "\n", "glfwInit");

	const int WINDOW_WIDTH = 1280, WINDOW_HEIGHT = 720;

	Window window("Vaulteer", WINDOW_WIDTH, WINDOW_HEIGHT);

	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glEnable(GL_DEBUG_OUTPUT);
	log.debug("GL_DEBUG_OUTPUT enabled.\n", "glEnable");

	glDebugMessageCallback(debugCallback, nullptr);
	log.debug("OpenGL debug callback set.\n", "glDebugMessageCallback");

	stbi_set_flip_vertically_on_load(true);
	log.debug("STBI vertical flip on load enabled.\n", "stbi_set_flip_vertically_on_load");

	glEnable(GL_DEPTH_TEST);
	log.debug("Depth test enabled.\n");
	Event::AddEventHandlingForWindow(&window);
	glfwSetInputMode(window.getRawWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//glEnable(GL_FRAMEBUFFER_SRGB);
	//log.debug("SRGB enabled.\n");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------

 	GBuffer gbuffer = GBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

	GeometryTechnique geometryTech("geometry_vertex.glsl", GL_VERTEX_SHADER, "geometry_frag.glsl", GL_FRAGMENT_SHADER);
	LightingTechnique lightingTech("deferred_vertex.glsl", GL_VERTEX_SHADER, "deferred_frag.glsl", GL_FRAGMENT_SHADER);
	ShadowTechnique shadowTech("depth_vertex.glsl", GL_VERTEX_SHADER, "depth_frag.glsl", GL_FRAGMENT_SHADER);

	ShadowTechnique shadowCascadeShader("shadow_cascade_vertex.glsl", GL_VERTEX_SHADER, "shadow_cascade_frag.glsl", GL_FRAGMENT_SHADER);
	ShadowCubeTechnique shadowCubeShader("shadow_cube_vertex.glsl", GL_VERTEX_SHADER, "shadow_cube_geometry.glsl", GL_GEOMETRY_SHADER, "shadow_cube_frag.glsl", GL_FRAGMENT_SHADER);

	Technique lightSourceShader("lightsource_vertex.glsl", GL_VERTEX_SHADER, "lightsource_frag.glsl", GL_FRAGMENT_SHADER);

	// bind texture type to named uniform (geometry_frag needs color info)
	Texture::uniformTextureTypes.emplace(aiTextureType_NONE, Binder::geometry_frag::uniforms::diffuse1);
	//Texture::uniformTextureTypes.emplace(aiTextureType_DIFFUSE, Binder::geometry_frag::uniforms::diffuse1);
	//Texture::uniformTextureTypes.emplace(aiTextureType_SPECULAR, Binder::geometry_frag::uniforms::specular1);

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Model model1("teapot.obj");
	Model model2("teapot.obj");
	Model model3("teapot.obj");
	Model model4("teapot.obj");
	Model cube("cube.obj");
	Model quad("quad.obj");
	model1.setShaderContext(&geometryTech);

	Scene scene = { cube, model1, model2, model3, model4 };

	unsigned int nullTextureID;
	glGenTextures(1, &nullTextureID);
	glBindTexture(GL_TEXTURE_2D, nullTextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	MyCamera camera(glm::vec3(.0f, 3.0f, -3.f), glm::vec3(.0f, .0f, 1.0f), glm::vec3(.0f, 1.0f, .0f), glm::vec3(.0f, .5f, 1.0f), WINDOW_WIDTH, WINDOW_HEIGHT);
	MyCamera cameraCopy(glm::vec3(.0f, 3.0f, -3.f), glm::vec3(.0f, .0f, 1.0f), glm::vec3(.0f, 1.0f, .0f), glm::vec3(.0f, .5f, 1.0f), WINDOW_WIDTH, WINDOW_HEIGHT);
	//camera.camera_far = 30.f;

	std::vector<float> cascadeBounds = { 1.0f, 25.0f, 60.0f, 150.0f };

	ShadowRenderer shadowRenderer = ShadowRenderer(shadowTech, shadowCubeShader, camera, cascadeBounds);

 	const int NUM_LIGHTS = 4;
	glm::vec3 lightPositions[NUM_LIGHTS];
	float lightColorOffsets[NUM_LIGHTS];
	int8_t lightDirs[NUM_LIGHTS];
	GLSLPointLight pointLights[NUM_LIGHTS];
	GLSLAttenuation att = { 1.0f, -0.20f, 0.062f }; // for no dropoff: { 1.0f, .0f, .0f }


	srand((int) glfwGetTime());

	for (int i = 0; i < NUM_LIGHTS; i++) {
		lightDirs[i] = (rand() % 2) * 2 - 1;
		float pos = randf(-100.0f, 10.0f);
		float posScale = 10.0f;
		lightPositions[i] = glm::vec3(cosf(pos) * posScale, randf(1.2f, 12.0f), sinf(pos) * posScale);
		lightColorOffsets[i] = 0.0f;
		pointLights[i] = { glm::vec3(1.0f), 0.1f, 0.4f, lightPositions[i], att };
		pointLights[i].radius = GLSLPointLight::calculateRadius(pointLights[i]);

		shadowRenderer.addPointBuffer(1024, pointLights[i]);
	}


	float event_check_f = false, event_check_g = false, event_check_c = false;
	int timeoutOrig = 40, timeout_f = 40, timeout_g = 40, timeout_c = 40;

	float deltaTime = 0, lastFrame = 0, startTime = glfwGetTime();
	std::cout << "Loaded in " << startTime << " seconds." << std::endl;

	float dirTime = 0.0f;
	
	while (window.is_running()) {

		dirTime = !event_check_f ? dirTime : glfwGetTime() / 4;

		//GLSLDirectionalLight dirLight = { glm::vec3(1.0f), 0.01f, 0.3f, glm::vec3(sinf(dirTime), -1.0f, cosf(dirTime)) };
		GLSLDirectionalLight dirLight = { glm::vec3(1.0f), 0.01f, 0.3f, glm::vec3(sinf(dirTime), -1.0f, cosf(dirTime)) };

		lastFrame = deltaTime;
		deltaTime = glfwGetTime() - lastFrame;

		glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		log.debug("Color buffer bit and depth bit cleared.\n", "glClear");

		// moving light
		//dirLight.direction = glm::vec3(sinf(glfwGetTime()), -1.0f, cosf(glfwGetTime()));
		glm::vec3 lightCurrentPos[NUM_LIGHTS];
		glm::vec3 lightCurrentColor[NUM_LIGHTS];
		for (int i = 0; i < NUM_LIGHTS; i++) {
			lightCurrentPos[i] = lightPositions[i];
			//lightCurrentColor[i] = getColor(fmod(lightColorOffsets[i] + glfwGetTime(), 3.0f));
		}

		MyCamera* cam = (event_check_c ? &cameraCopy : &camera);

		// geometry pass
        // -----------------------------------------------------------------------------------------------------------------------

		gbuffer.bindForWriting();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		geometryTech.use();

		geometryTech.setView(camera.getStaticView());
		geometryTech.setProjection(camera.getStaticProjection());
		geometryTech.setTexture(nullTextureID);

 		model1.setPosition(0 * 3.f, 2.0f, 0 * 8.f);
 		model2.setPosition(0 * 3.f, 2.0f, 1 * 8.f);
 		model3.setPosition(0 * 3.f, 2.0f, 2 * 8.f);
 		model4.setPosition(0 * 3.f, 2.0f, 3 * 8.f);

 		model1.draw(geometryTech);
		model2.draw(geometryTech);
		model3.draw(geometryTech);
		model4.draw(geometryTech);

		cube.setScale(100.0f, 1.0f, 100.0f);
		cube.draw(geometryTech);

		// unbind gbuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// shadow map pass
		// -----------------------------------------------------------------------------------------------------------------------

		shadowRenderer.setCamera(*cam);
		shadowRenderer.updateCascadeBounds(dirLight.direction);

		for (int i = 0; i < NUM_LIGHTS; i++) {
			shadowRenderer.getPointBuffer(i).lightPos = pointLights[i].position;
		}

		shadowRenderer.render(scene);

		// light pass
        // -----------------------------------------------------------------------------------------------------------------------

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		lightingTech.use();

		for (int i = 0; i < GBuffer::GBufferTextureType::NumTextures; i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, gbuffer.textures[i]);
		}
		for (int i = 0; i < shadowRenderer.numCascades; i++) {
			glActiveTexture(GL_TEXTURE0 + GBuffer::GBufferTextureType::NumTextures + i);
			glBindTexture(GL_TEXTURE_2D, shadowRenderer.getCascadeBuffer(i).getTextureId());
		}
		for (int i = 0; i < shadowRenderer.numPointBuffers; i++) {
			GLenum texUnit = GBuffer::GBufferTextureType::NumTextures + shadowRenderer.numCascades + i;
			glActiveTexture(GL_TEXTURE0 + texUnit);
			glBindTexture(GL_TEXTURE_CUBE_MAP, shadowRenderer.getPointBuffer(i).getTextureId());
		}


		for (int i = 0; i < NUM_LIGHTS; i++) {
			//pointLights[i].position = lightCurrentPos[i];
			//pointLights[i].color = lightCurrentColor[i];
			lightingTech.setPointLight(pointLights[i], i);
		}
		
		lightingTech.setDirectionalLight(dirLight);

		lightingTech.setWorldCameraPos(camera.position);
		lightingTech.setCameraViewMat(camera.getViewMatrix());
		lightingTech.setShadowMapData(shadowRenderer);

		//lightingTech.setFogColor(glm::vec3(getColor(fmod(glfwGetTime(), 3.0f))));

		lightingTech.setMaterialSpecularIntensity(1.0f);
		lightingTech.setMaterialShininess(32.0f);

		if (event_check_g) {
			shadowCascadeShader.use();
			shadowCascadeShader.setUniform(Binder::shadow_cascade_frag::uniforms::depthMap, 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, shadowRenderer.getCascadeBuffer(0).getTextureId());

			quad.draw(shadowCascadeShader);
		}
		else {
			quad.draw(lightingTech);
		}
		
		// render quad to default framebuffer
		//quad.draw(lightingTech);
		
		glClear(GL_DEPTH_BUFFER_BIT);
		
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
		lightSourceShader.setUniform(Binder::lightsource_vertex::uniforms::view, 1, GL_FALSE, camera.getViewMatrix());
		lightSourceShader.setUniform(Binder::lightsource_vertex::uniforms::projection, 1, GL_FALSE, camera.getProjectionMatrix());

		for (int i = 0; i < NUM_LIGHTS; i++) {
			glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), lightCurrentPos[i] + glm::vec3(0, -0.05, 0));
			modelMat = glm::scale(modelMat, glm::vec3(0.1f));
			modelMat = glm::rotate(modelMat, (float)glfwGetTime() * 3, glm::vec3(0.0f, 1.0f, 0.0f));

			lightSourceShader.setUniform(Binder::lightsource_vertex::uniforms::model, 1, GL_FALSE, modelMat);
			lightSourceShader.setUniform(Binder::lightsource_frag::uniforms::lightColor, 1,  pointLights[i].color);

			model1.draw(lightSourceShader);
		}
		

		if (false) {
			ShadowCascade& shadowCascade = shadowRenderer.getCascade(0);

			lightSourceShader.use();
			lightSourceShader.setUniform(Binder::lightsource_vertex::uniforms::view, 1, GL_FALSE, camera.getViewMatrix());
			lightSourceShader.setUniform(Binder::lightsource_vertex::uniforms::projection, 1, GL_FALSE, camera.getProjectionMatrix());


			for (int i = 4; i < shadowCascade.cameraFrustumWS.size(); i++) {
				glm::vec3 pos = glm::vec3(shadowCascade.cameraFrustumWS[i].x, shadowCascade.cameraFrustumWS[i].y, shadowCascade.cameraFrustumWS[i].z);
				glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), pos + glm::vec3(0, 0.1f / 2, 0));
				modelMat = glm::scale(modelMat, glm::vec3(1.f) * 0.2f);
				lightSourceShader.setUniform(Binder::lightsource_vertex::uniforms::model, 1, GL_FALSE, modelMat);
				lightSourceShader.setUniform(Binder::lightsource_frag::uniforms::lightColor, 1, glm::vec3(0.2f, 0.2f, 1.0f));
				cube.draw(lightSourceShader);
			}

			for (int i = 0; i < 8; i++) {
				glm::vec3 pos = shadowCascade.cascadeCorners[i];
				glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), pos);
				modelMat = glm::scale(modelMat, glm::vec3(0.2f));
				lightSourceShader.setUniform(Binder::lightsource_vertex::uniforms::model, 1, GL_FALSE, modelMat);
				lightSourceShader.setUniform(Binder::lightsource_frag::uniforms::lightColor, 1, glm::vec3(1.0f, 0.2f, 0.2f));
				cube.draw(lightSourceShader);
			}
		}

		glfwSwapBuffers(window.getRawWindow());

		Event::Poll();
		handleCamera(camera);


		if (timeout_f-- < 0 && Event::Check << (Event::Key == Event::KEY::F && Event::Action == Event::ACTION::PRESS)) {
			event_check_f = !event_check_f;
			timeout_f = timeoutOrig;
		}
		if (timeout_g-- < 0 && Event::Check << (Event::Key == Event::KEY::G && Event::State == Event::STATE::DOWN)) {
			event_check_g = !event_check_g;
			timeout_g = timeoutOrig;
		}
		if (timeout_c-- < 0 && Event::Check << (Event::Key == Event::KEY::C && Event::State == Event::STATE::DOWN)) {
			event_check_c = !event_check_c;
			timeout_c = timeoutOrig;

			cameraCopy.position = camera.position;
			cameraCopy.front = camera.front;
			cameraCopy.right = camera.right;
		}

		DebugLogger<>::disableLogging();
	}

	DebugLogger<>::setDefaultMessageNameMessageLimit(0);
	DebugLogger<>::setMessageNameMessageLimit("glUniform1i", 1000);
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

