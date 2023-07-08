#pragma once
#include "API/Layer.h"
#include "API/CameraController.h"

#include "Renderer/Renderer.h"
#include "Renderer/ForwardRenderer.h"
#include "Renderer/ShadowVolumeRenderer.h"

#include "API/Application.h"
#include "Model/Resources/ResourceManager.h"
#include "Renderer/TextRenderer.h"
#include "Utils/MathUtils.h"

class WorldLayer : public Layer {
public:
	WorldLayer() : Layer("WorldLayer") {}
	~WorldLayer() override = default;
	
	Camera camera;
	CameraController cameraController;

	std::vector<std::unique_ptr<Model>> loadedModels;
	std::vector<std::unique_ptr<Mesh>> copies;
	std::vector<std::vector<Mesh*>> something;

	std::vector<PointLight> lights;
	std::vector<Entity> lightEntities;

	//Scenes
	inline static constexpr size_t scene_0 = 0;
	Scene<scene_0> scene;

	Renderer<DeferredRenderer, BlendingForwardRenderer, TextRenderer> renderer;

	void onAttach() override {
		Window& window = Application::getWindow();

		auto setAspectRatio = [this](int w, int h) {
			camera.propertiesCamera.aspectRatio = (float)w / h;
		};
 		Window::addResizeCallback(setAspectRatio);
		
		ResourcePack& pack = ResourceManager::getPack(0);
		

		Entity& lightEntity = lightEntities.emplace_back();
		DirectionalLight light = {
			{glm::vec3(1.0f, 0.01f, 0.01f), 0.01f, 1.0f},
			glm::vec3(0.5f, -1.f, -0.5f)};
		lightEntity.add<DirectionalLight>(light);
		scene.add(lightEntity);
		
		Entity& lightEntity2 = lightEntities.emplace_back();
		DirectionalLight light2 = {
			{glm::vec3(0.01f, 0.01f, 1.0f), 0.01f, 1.0f},
			glm::vec3(0.4f, -1.f, -0.6f)};
		lightEntity2.add<DirectionalLight>(light2);
		scene.add(lightEntity2);
		
		Entity& lightEntity3 = lightEntities.emplace_back();
		DirectionalLight light3 = {
			{glm::vec3(0.01f, 1.0f, 0.01f), 0.01f, 1.0f},
			glm::vec3(0.6f, -1.f, -0.4f)};
		lightEntity3.add<DirectionalLight>(light3);
		scene.add(lightEntity3);


		ForwardRenderer::initialize(pack.getTextureID());
		DeferredRenderer::initialize(pack.getTextureID(), Window::getWidth(), Window::getHeight());
		BlendingForwardRenderer::initialize(pack.getTextureID(), Window::getWidth(), Window::getHeight());
		ShadowVolumeRenderer::initialize(light);
		TextRenderer::initialize(Window::getWidth(), Window::getHeight());

		
		camera.enableAxisLock();
		camera.setLockAxis({0,1,0});

		camera.propertiesCamera.aspectRatio = (double)Window::getWidth()/Window::getHeight();
		camera.propertiesCamera.fov = 60;
		camera.propertiesCamera.near = 0.1f;
		camera.propertiesCamera.far = 1000;

		cameraController.setCamera(&camera);

		scene.activeCamera = &camera;
		scene.add(camera);


		Model& palm = *loadedModels.emplace_back(std::make_unique<Model>(pack.getMeshes("palm")));
		Model& teapot = *loadedModels.emplace_back(std::make_unique<Model>(pack.getMeshes("quad")));


		Model& quad = *loadedModels.emplace_back(std::make_unique<Model>(pack.getMeshes("quad")));
		Model& quad2 = *loadedModels.emplace_back(std::make_unique<Model>(pack.getMeshes("quad")));
		Model& crate1 = *loadedModels.emplace_back(std::make_unique<Model>(pack.getMeshes("crate")));
		Model& crate2 = *loadedModels.emplace_back(std::make_unique<Model>(pack.getMeshes("crate")));
		Model& crate3 = *loadedModels.emplace_back(std::make_unique<Model>(pack.getMeshes("crate")));

		palm.add<Shadow>();
		teapot.add<Shadow>();
		crate1.add<Shadow>();
		
		crate1.setRotation(M_PI_4,M_PI_4 / 3 * 2,M_PI_4);


		quad.setPosition({0.f, -2.f, 0.f});
		quad.setRotation(-M_PI_2, 0.f, 0.f);
		quad.setScale({100.f, 1.f, 100.f});

		quad2.setPosition({0.f, 0.f, -10.f});
		quad2.setRotation(0.f, 0.f, -M_PI_2);
		quad2.setScale({100.f, 100.f, 1.f});


		palm.setPosition(glm::vec3(0, 0, -5));
		palm.setScale(glm::vec3(0.5f));

		teapot.setPosition(glm::vec3(0, 0, 5));

		crate1.setPosition(glm::vec3(5, 0, 0));

		crate2.setPosition(glm::vec3(20, 2, 0));
		crate2.setScale(glm::vec3(0.3f));
		crate3.setPosition(glm::vec3(0, 2, 20));
		crate3.setScale(glm::vec3(0.3f));

		crate2.setRotation(M_PI_4,M_PI_4 / 3 * 2,M_PI_4);

		// should this not be done automatically on construction?
		for (auto& model : loadedModels) {
			scene.add(*model);
		}

		
		UniformBufferTechnique::uploadMaterialData();
		UniformBufferTechnique::uploadTextureData();
 		UniformBufferTechnique::uploadTextureViewData();
		UniformBufferTechnique::uploadCameraProjection(scene.getActiveCamera().projectionMatrix());

		DeferredRenderer::buildLights(scene);

		glClearColor(0.001f, 0.001f, 0.001f, 1.0f);
	}

	float fps = 0.0f;
	float drawCalls = 0.0f;
	float cumTime = 0.0f;
	uint cumFrames = 0;
	uint cumDrawCalls = 0;

	bool update = false;
	std::string content;
	Justify justify = Justify::Right;
	float scale = 1.0;

	void onUpdate(float timestep) override {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		DeferredRenderer::resetStats();
		TextRenderer::resetStats();

		cameraController.onUpdate(timestep);

		cumTime += timestep;
		cumFrames++;
		if (cumTime > 0.5f) {
			fps = (float)cumFrames / cumTime;
			drawCalls = (float)cumDrawCalls / cumFrames;
			cumFrames = 0;
			cumDrawCalls = 0;
			cumTime = 0.0f;
		}

		std::string timer = std::format("FPS {:.1f}", fps);
		std::string calls = std::format("Draws per frame: {:.1f}", drawCalls);
		TextRenderer::submitText(timer, {20, 27}, 0.5);
		TextRenderer::submitText(calls, {20, 50}, 0.5);

		glm::mat4 view = scene.getActiveCamera().viewMatrix();
		UniformBufferTechnique::uploadCameraView(view);

		renderer.render(scene);
		
		cumDrawCalls += DeferredRenderer::stats.drawCalls;
		cumDrawCalls += TextRenderer::stats.drawCalls;
		TextRenderer::clearScene();
	}

	bool onKeyboardPressEvent(KeyboardButtonEvent& e) {
		if (e.button.action == KeyAction::PRESS && e.button.key == KeyboardKey::Q) {
			DeferredRenderer::drawVolumes = !DeferredRenderer::drawVolumes;
		}

		return true;
	}

	void onEvent(BaseEvent& e) override {
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseMoveEvent>(FORWARD_FN(cameraController.onMouseMoveEvent));
		dispatcher.dispatch<KeyboardButtonEvent>(FORWARD_FN(cameraController.onKeyboardButtonEvent));
		dispatcher.dispatch<KeyboardButtonEvent>(FORWARD_FN(onKeyboardPressEvent));
	}
};

