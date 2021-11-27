
#define STB_IMAGE_IMPLEMENTATION

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
#include "ShadowMapTechnique.h"
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

void z() {
	getFrustumCorners(glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 100.0f));
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

	GeometryTechnique geometryTech("shaders/geometry_vertex.glsl", GL_VERTEX_SHADER, "shaders/geometry_frag.glsl", GL_FRAGMENT_SHADER);
	LightingTechnique lightingTech("shaders/deferred_vertex.glsl", GL_VERTEX_SHADER, "shaders/deferred_frag.glsl", GL_FRAGMENT_SHADER);

	ShadowTechnique shadowShader("shaders/shadow_vertex.glsl", GL_VERTEX_SHADER, "shaders/shadow_frag.glsl", GL_FRAGMENT_SHADER);
	ShadowCubeTechnique shadowCubeShader("shaders/shadow_cube_vertex.glsl", GL_VERTEX_SHADER, "shaders/shadow_cube_geometry.glsl", GL_GEOMETRY_SHADER, "shaders/shadow_cube_frag.glsl", GL_FRAGMENT_SHADER);

	ShadowMapTechnique shadowMapShader("shaders/depth_map_vertex.glsl", GL_VERTEX_SHADER, "shaders/depth_map_frag.glsl", GL_FRAGMENT_SHADER);

	Technique shadowSpotlightShader("shaders/shadow_cascade_vertex.glsl", GL_VERTEX_SHADER, "shaders/shadow_cascade_frag.glsl", GL_FRAGMENT_SHADER);

	Technique lightSourceShader("shaders/lightsource_vertex.glsl", GL_VERTEX_SHADER, "shaders/lightsource_frag.glsl", GL_FRAGMENT_SHADER);

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

	ModelVec scene = { cube, model1, model2, model3, model4 };

	unsigned int nullTextureID;
	glGenTextures(1, &nullTextureID);
	glBindTexture(GL_TEXTURE_2D, nullTextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	MyCamera camera(glm::vec3(.0f, 3.0f, -3.f), glm::vec3(.0f, .0f, 1.0f), glm::vec3(.0f, 1.0f, .0f), glm::vec3(.0f, .5f, 1.0f), WINDOW_WIDTH, WINDOW_HEIGHT);
	MyCamera cameraCopy(glm::vec3(.0f, 3.0f, -3.f), glm::vec3(.0f, .0f, 1.0f), glm::vec3(.0f, 1.0f, .0f), glm::vec3(.0f, .5f, 1.0f), WINDOW_WIDTH, WINDOW_HEIGHT);
	//camera.camera_far = 30.f;

	std::vector<float> cascadeBounds = { camera.camera_near, 25.0f, 60.0f, 150.0f };

	ShadowRenderer shadowRenderer = ShadowRenderer(camera, cascadeBounds);

	const int NUM_LIGHTS = 4;
	glm::vec3 lightPositions[NUM_LIGHTS];
	glm::vec3 lightColors[NUM_LIGHTS];

	GLSLPointLight pointLights[NUM_LIGHTS];
	GLSLSpotLight spotLights[1];

	GLSLAttenuation att = { 1.0f, 0.02f, 0.062f }; // for no dropoff: { 1.0f, .0f, .0f }

	lightColors[0] = glm::vec3(0.f, 0.f, 1.f);
	lightColors[1] = glm::vec3(0.f, 1.f, 0.f);
	lightColors[2] = glm::vec3(1.f, 0.f, 0.f);
	lightColors[3] = glm::vec3(1.f, 1.f, 0.f);

	srand((int)glfwGetTime());

	for (int i = 0; i < NUM_LIGHTS; i++) {
		float pos = randf(-100.0f, 10.0f);
		float posScale = 10.0f;
		lightPositions[i] = glm::vec3(cosf(pos) * posScale, randf(1.2f, 12.0f), sinf(pos) * posScale);

		pointLights[i] = { glm::vec3(1.0f), 0.1f, 0.4f, lightPositions[i], att};
		 
		shadowRenderer.addPointBuffer(1024, pointLights[i]);
	}

	glm::vec3 dir(-0.423556805, -0.121436201, 0.897693098);
	glm::vec3 pos(8.84644413, 5.45251179, -12.6091557);
	dir = glm::vec3(0.f, -1.f, 0.f);
	pos = glm::vec3(0.f, 8.f, 0.f); 

	spotLights[0] = {glm::vec3(1.0f),  0.1f, 1.0f, pos, att, dir, M_PI / 4 };
	shadowRenderer.addSpotBuffer(spotLights[0]);

	float event_check_f = false, event_check_g = false, event_check_c = false;
	int timeoutOrig = 40, timeout_f = 40, timeout_g = 40, timeout_c = 40;

	float deltaTime = 0, lastFrame = 0, startTime = glfwGetTime();
	std::cout << "Loaded in " << startTime << " seconds." << std::endl;

	float dirTime = 0.0f;
	
	while (window.is_running()) {

		dirTime = !event_check_f ? dirTime : glfwGetTime() / 4;

		//GLSLDirectionalLight dirLight = { glm::vec3(1.0f), 0.01f, 0.3f, glm::vec3(sinf(dirTime), -1.0f, cosf(dirTime)) };
		GLSLDirectionalLight dirLight = { glm::vec3(1.0f), 0.01f, 0.3f, glm::vec3(sinf(dirTime), -1.0f, cosf(dirTime)) };

		if (event_check_c) {
			spotLights[0].position = camera.position;
			spotLights[0].direction = camera.front;
		}

		lastFrame = deltaTime;
		deltaTime = glfwGetTime() - lastFrame;

		glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		log.debug("Color buffer bit and depth bit cleared.\n", "glClear");

		// moving light
		glm::vec3 lightCurrentPos[NUM_LIGHTS];
		glm::vec3 lightCurrentColor[NUM_LIGHTS];
		for (int i = 0; i < NUM_LIGHTS; i++) {
			lightCurrentPos[i] = lightPositions[i];
			lightCurrentColor[i] = lightColors[i];
		}

		MyCamera* cam = (false ? &cameraCopy : &camera);

		// geometry pass
        // -----------------------------------------------------------------------------------------------------------------------

		gbuffer.bindForWriting();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		geometryTech.use();

		geometryTech.setProjection(camera.getStaticProjection());
 		geometryTech.setTexture(nullTextureID);

 		model1.setPosition(0 * 3.f, 2.0f, 0 * 8.f);
 		model2.setPosition(0 * 3.f, 2.0f, 1 * 8.f);
 		model3.setPosition(0 * 3.f, 2.0f, 2 * 8.f);
 		model4.setPosition(0 * 3.f, 2.0f, 3 * 8.f);

		geometryTech.setModelView(model1.getModelMat(), camera.getStaticView());
 		model1.draw(geometryTech);
		geometryTech.setModelView(model2.getModelMat(), camera.getStaticView());
		model2.draw(geometryTech);
		geometryTech.setModelView(model3.getModelMat(), camera.getStaticView());
		model3.draw(geometryTech);
		geometryTech.setModelView(model4.getModelMat(), camera.getStaticView());
		model4.draw(geometryTech);

		cube.setScale(100.0f, 1.0f, 100.0f);
		geometryTech.setModelView(cube.getModelMat(), camera.getStaticView());
		cube.draw(geometryTech);

		// unbind gbuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// shadow map pass
		// -----------------------------------------------------------------------------------------------------------------------

		shadowRenderer.setCamera(*cam);
		shadowRenderer.updateCascadeBounds(dirLight.direction);

		// per frame update of light position in shadow realm
		for (int i = 0; i < NUM_LIGHTS; i++) {
			if (shadowRenderer.numPointBuffers > i)
				shadowRenderer.getPointBuffer(i).lightPos = pointLights[i].position;
		}
		for (int i = 0; i < 1; i++) {
			shadowRenderer.updateSpotLight(i, spotLights[i]);
		}

		shadowRenderer.renderCascades(scene, shadowShader);
		shadowRenderer.renderPointLights(scene, shadowCubeShader);
		shadowRenderer.renderSpotLights(scene, shadowShader);

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
		for (int i = 0; i < shadowRenderer.numSpotBuffers; i++) {
			GLenum texUnit = GBuffer::GBufferTextureType::NumTextures + shadowRenderer.numCascades + shadowRenderer.numPointBuffers;
			glActiveTexture(GL_TEXTURE0 + texUnit);
			glBindTexture(GL_TEXTURE_2D, shadowRenderer.getSpotBuffer(i).getTextureId());
		}


		//lightingTech.setPointLight(pointLights[0], 0);
		
		lightingTech.setSpotLight(spotLights[0], 0);
		
		//lightingTech.setDirectionalLight(dirLight);

		lightingTech.setWorldCameraPos(camera.position);
		lightingTech.setCameraViewMat(camera.getStaticView());
		lightingTech.setShadowMapData(shadowRenderer);

		//lightingTech.setFogColor(glm::vec3(getColor(fmod(glfwGetTime(), 3.0f))));

		lightingTech.setMaterialSpecularIntensity(1.0f);
		lightingTech.setMaterialShininess(32.0f);

		if (event_check_g) {
			shadowMapShader.use();
			shadowMapShader.setUniform(Binder::depth_map_frag::uniforms::depthMap, 0);
			glActiveTexture(GL_TEXTURE0);
 			glBindTexture(GL_TEXTURE_2D, shadowRenderer.getSpotBuffer(0).getTextureId());

			quad.draw(shadowMapShader);
		}
		else {
			quad.draw(lightingTech);
		}
		
		// render quad to default framebuffer
		//quad.draw(lightingTech);
		
		glClear(GL_DEPTH_BUFFER_BIT);
		
		/*
		// 2.5. copy content of geometry's depth buffer to default framebuffer's depth buffer
		// ----------------------------------------------------------------------------------
		gbuffer.bindForReading();
		
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
		// blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
		// the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the 		
		// depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
		glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		*/

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
		
		// visualization for cascade bounds
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

		if (true) {
			GLSLSpotLight spotLight = spotLights[0];

			lightSourceShader.use();
			lightSourceShader.setUniform(Binder::lightsource_vertex::uniforms::view, 1, GL_FALSE, camera.getViewMatrix());
			lightSourceShader.setUniform(Binder::lightsource_vertex::uniforms::projection, 1, GL_FALSE, camera.getProjectionMatrix());

			std::vector<glm::vec4> frustumCorners = getFrustumCornersWorldSpace(GLSLSpotLight::getViewMatrix(spotLight),
																				GLSLSpotLight::getProjectionMatrix(spotLight));

 			for (int i = 0; i < frustumCorners.size(); i++) {
				float scale = (i < 4 ? 0.02f : 0.2f);

				glm::vec3 pos = glm::vec3(frustumCorners[i].x, frustumCorners[i].y, frustumCorners[i].z);
				glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), pos + glm::vec3(0, scale / 2, 0));
				modelMat = glm::scale(modelMat, glm::vec3(1.f) * scale);
				lightSourceShader.setUniform(Binder::lightsource_vertex::uniforms::model, 1, GL_FALSE, modelMat);
				lightSourceShader.setUniform(Binder::lightsource_frag::uniforms::lightColor, 1, glm::vec3(0.2f, 0.2f, 1.0f));
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

