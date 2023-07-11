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
		
		/*
		Entity& lightEntity = lightEntities.emplace_back();
		DirectionalLight dirLight = {
			{glm::vec3(0.3f, 0.6f, 0.2f), 0.01f, 1.0f},
			glm::vec3(0.0f, -1.0f, -1.0f)};
		lightEntity.add<DirectionalLight>(dirLight);
		scene.add(lightEntity);
		 */


		ForwardRenderer::initialize(pack.getTextureID());
		DeferredRenderer::initialize(pack.getTextureID(), Window::getWidth(), Window::getHeight());
		BlendingForwardRenderer::initialize(pack.getTextureID(), Window::getWidth(), Window::getHeight());
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

		Model& quad = *loadedModels.emplace_back(std::make_unique<Model>(pack.getMeshes("quad")));
		Model& quad2 = *loadedModels.emplace_back(std::make_unique<Model>(pack.getMeshes("quad")));


		palm.add<Shadow>();
		


		quad.setPosition({0.f, -2.f, 0.f});
		quad.setRotation(-M_PI_2, 0.f, 0.f);
		quad.setScale({100.f, 1.f, 100.f});

		quad2.setPosition({0.f, 0.f, -10.f});
		quad2.setRotation(0.f, 0.f, -M_PI_2);
		quad2.setScale({100.f, 100.f, 1.f});


		palm.setPosition(glm::vec3(0, 0, -5));
		palm.setScale(glm::vec3(0.5f));

		
		std::vector<glm::mat4> palmInstance = { palm.getModelMatrix() };
		palm.useInstancing(palmInstance);

		int plane = 20;
		int dispersion = 2;
		
		std::vector<glm::vec3> lightColors = {
			{1.0, 0.01, 0.01},
			{1.0, 1.0, 0.01},
			{0.01, 1.0, 0.01},
			{0.01, 1.0, 1.0},
			{0.01, 0.01, 1.0},
			{1.0, 0.01, 1.0},
		};

		Material copy = *MaterialLibrary::get(0);
		copy.data.colorAmbient = 0.9f;
		copy.data.matOpacity = 0.1f;
		Material* transparent = MaterialLibrary::create(copy.data, "cratei");

		for (int y = 0; y < plane * dispersion; y += 2 * dispersion) {
			for (int x = -plane * dispersion; x < plane * dispersion; x += 2 * dispersion) {
				if (rand() % 8 == 0) {
					Model& crate = *loadedModels.emplace_back(std::make_unique<Model>(pack.getMeshes("crate")));
					crate.setPosition(glm::vec3(x, y, 10.0f));
					crate.setScale(glm::vec3(0.2f));
					crate.setMaterial(transparent);

					BaseLight base = {lightColors[rand() % 6], 0.13f, 0.5f};
					Attenuation att = { 1.0f, 0.18f, 0.032f };
					PointLight& pointLight = lights.emplace_back(att, base, *crate.position);
					crate.add<PointLight>(pointLight);
				}
			}
		}
		
		Model& cratei = *loadedModels.emplace_back(std::make_unique<Model>(pack.getMeshes("cratei")));
		cratei.add<Shadow>();
		std::vector<glm::mat4> instances;

		for (int y = 0; y < plane * dispersion; y += 1 * dispersion) {
			for (int x = -plane * dispersion; x < plane * dispersion; x += 1 * dispersion) {
				if (rand() % 8 == 0) {
					Position3D pos = glm::vec3(x, y, 0.0f);
					Rotation3D rot = Rotation3D({rand() % 8 * M_PI_4, rand() % 8 * M_PI_4, rand() % 8 * M_PI_4});
					Properties3D props = {.scale = glm::vec3(1.0f)};
					instances.push_back(Object3D::modelMatrix(pos, rot, props));
				}
			}
		}
		cratei.useInstancing(instances);
		
		// should this not be done automatically on construction?
		for (auto& model : loadedModels) {
			model->addRenderComponents();
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
	float textScale = 0.5;

	void onUpdate(float timestep) override {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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
		TextRenderer::submitText(timer, {20, 27}, textScale);
		TextRenderer::submitText(calls, {20, 50}, textScale);

		glm::mat4 view = scene.getActiveCamera().viewMatrix();
		UniformBufferTechnique::uploadCameraView(view);

		renderer.render(scene);

		cumDrawCalls += renderer.getNumDrawCalls();
		renderer.resetStats();

		TextRenderer::clearScene();
	}

	bool onKeyboardButtonEvent(KeyboardButtonEvent& e) {
		return true;
	}

	void onEvent(BaseEvent& e) override {
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseMoveEvent>(FORWARD_FN(cameraController.onMouseMoveEvent));
		dispatcher.dispatch<KeyboardButtonEvent>(FORWARD_FN(cameraController.onKeyboardButtonEvent));
		dispatcher.dispatch<KeyboardButtonEvent>(FORWARD_FN(onKeyboardButtonEvent));
	}
};

