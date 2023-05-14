#pragma once
#include "API/Layer.h"
#include "API/CameraController.h"

#include "Renderer/Renderer.h"
#include "Renderer/BlendingForwardRenderer.h"
#include "Renderer/DeferredRenderer.h"

#include "API/Application.h"
#include "Model/Resources/ResourceManager.h"
#include "Utils/MathUtils.h"

class WorldLayer : public Layer {
public:
	WorldLayer() : Layer("WorldLayer") {}
	~WorldLayer() override = default;

private:
	Camera camera;
	CameraController cameraController;

	std::vector<Model> loadedModels;

	//Scenes
	inline static constexpr size_t scene_0 = 0;
	Scene<scene_0> scene;

	//StaticScene<Model<ModelData>> staticScene;
	//StaticScene<OpaqueModel, Model<LineData>> opaqueScene;
	//StaticScene<TransparentModel> transparentScene;
	
	Renderer<BlendingForwardRenderer> renderer; // render todo
	//Renderer<DeferredRenderer, BlendingForwardRenderer> renderer; // render my nuts

public:
	void onAttach() override {
		Window& window = Application::getWindow();

		DeferredRenderer::initialize(Window::getWidth(), Window::getHeight());
		BlendingForwardRenderer::initialize(Window::getWidth(), Window::getHeight());

		//Setting up cameras in the scene.
		
		camera.enableAxisLock({ 0,1,0 });
		camera.setRotation({ 0,0,0 });
		camera.propertiesCamera.aspectRatio = (double)Window::getHeight() / Window::getWidth();
		camera.propertiesCamera.fov = 60;
		camera.propertiesCamera.near = 0.1f;
		camera.propertiesCamera.far = 1000;

		cameraController.setCamera(&camera);

		scene.activeCamera = camera.entity_id;

		scene.add(camera);

		auto setAspectRatio = [this](int w, int h) {
			camera.propertiesCamera.aspectRatio = (float)h / w;
		};

		Window::addResizeCallback(setAspectRatio);
		
		ResourcePack& pack = ResourceManager::getPack(0);

		Model palm = Model(pack.getMeshes("palm"));
		Model crate = Model(pack.getMeshes("crate"));
		loadedModels.push_back(std::move(palm));
		loadedModels.push_back(std::move(crate));

		*loadedModels[0].position = glm::vec3(1);
		*loadedModels[1].position = glm::vec3(2);
		
		scene.add(loadedModels[0]);
		scene.add(loadedModels[1]);

		//auto model1 = Model(pack.getModelByName("palm"));
		//auto model2 = Model(pack.getModelByName("crate"));

		//DeferredRenderer::preload(pack);

		//Add models to scene layers(s)
		//std::vector<Object3D*> objects;

		//std::vector<OpaqueModel> opaqueModels;

		//Generate trees how about you kill yourself
		/*
		intmax_t width = 10, height = 10;
		for (intmax_t i = -(ceil(width / 2.0f)); i < (ceil(width / 2.0f)); i++) {
			for (intmax_t j = -(ceil(height / 2.0f)); j < (ceil(height / 2.0f)); j++) {
				float r = randf(1.0, 25.0);
				float x = sin(randf(0.f, M_2_PI)) * r;
				float y = cos(randf(0.f, M_2_PI)) * r;

				model1.setPosition(8 * i + x, ((float)(rand() % 8)/8), 8 * j + y);
				model1.setRotation((float) (rand() % 360) / M_PI, glm::vec3(0, 1, 0));
				opaqueScene.addObject(std::move(OpaqueModel(model1)), model1.getBoundingSphere());
			}
		}
		*/
		/*
		model2.setPosition(0, 0, 0);
		model2.setScale(glm::vec3(5.0f));
		model2.setRotation(0.0, glm::vec3(1.0, 0.0, 0));
		opaqueScene.addObject(std::move(OpaqueModel(model2)), model2.getBoundingSphere());
		opaqueScene.addObject(std::move(OpaqueModel(model2)), model2.getBoundingSphere());

		opaqueScene.finalize();

		model2.setPosition(20, 2, 0);
		model2.setScale(glm::vec3(0.5f));
		transparentScene.addObject(std::move(TransparentModel(model2)), model2.getBoundingSphere());
		model2.setPosition(0, 2, 20);
		transparentScene.addObject(std::move(TransparentModel(model2)), model2.getBoundingSphere());

		transparentScene.finalize();
		*/

		glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
	}

	void onUpdate(float timestep) override {
		cameraController.onUpdate(timestep);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		renderer.render(scene);
	}

	void onEvent(BaseEvent& e) override {
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseMoveEvent>(FORWARD_FN(cameraController.onMouseMoveEvent));
		dispatcher.dispatch<KeyboardButtonEvent>(FORWARD_FN(cameraController.onKeyboardButtonEvent));

	}
};

