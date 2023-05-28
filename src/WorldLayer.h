#pragma once
#include "API/Layer.h"
#include "API/CameraController.h"

#include "Renderer/Renderer.h"
#include "Renderer/ForwardRenderer.h"

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

	//Scenes
	inline static constexpr size_t scene_0 = 0;
	Scene<scene_0> scene;

	Renderer<DeferredRenderer, BlendingForwardRenderer, TextRenderer> renderer;
	//Renderer<ForwardRenderer> renderer;

	void onAttach() override {
		Window& window = Application::getWindow();

		auto setAspectRatio = [this](int w, int h) {
			camera.propertiesCamera.aspectRatio = (float)w / h;
		};
 		Window::addResizeCallback(setAspectRatio);
		
		ResourcePack& pack = ResourceManager::getPack(0);

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
		Model& crate1 = *loadedModels.emplace_back(std::make_unique<Model>(pack.getMeshes("crate")));
		Model& crate2 = *loadedModels.emplace_back(std::make_unique<Model>(pack.getMeshes("crate")));
		Model& crate3 = *loadedModels.emplace_back(std::make_unique<Model>(pack.getMeshes("crate")));


		palm.setPosition(glm::vec3(0, 0, -5));
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

		/*
		auto mat1 = palm.meshes->at(0)->material->data;
		mat1.matOpacity = 0.1;
		auto mat2 = palm.meshes->at(1)->material->data;
		mat2.matOpacity = 0.1;
		auto* ins1 = MaterialLibrary::create(mat1, "palm0_transparent");
		auto* ins2 = MaterialLibrary::create(mat2, "palm1_transparent");

		for (int y = 0; y < 100; y++) {
			for (int x = 0; x < 100; x++) {
				Model& a = *loadedModels.emplace_back(std::make_unique<Model>(pack.getMeshes("palm")));

				a.setPosition(y*4 - 100, -(rand() % 6), x*4 - 100);
				if ((y + x % 16) % 16 == 0) {
					a.setMaterial(ins1, 0);
					a.setMaterial(ins2, 1);
					a.add<Transparent>();
				}
			}
			
		}
		*/


		//Generate trees how about you kill yourself
		/*
		intmax_t width = 10, height = 10;
		for (intmax_t i = -(ceil(width / 2.0f)); i < (ceil(width / 2.0f)); i++) {
			for (intmax_t j = -(ceil(height / 2.0f)); j < (ceil(height / 2.0f)); j++) {
				float r = randf(1.0, 25.0);
				float x = sin(randf(0.f, M_2_PI)) * r;
				float y = cos(randf(0.f, M_2_PI)) * r;
				
				Model& model = *loadedModels.emplace_back(std::make_unique<Model>(pack.getMeshes("palm")));
				model.setPosition({8 * i + x, ((float)(rand() % 8)/8), 8 * j + y});
				model.setRotation((float) (rand() % 360) / M_PI, glm::vec3(0, 1, 0));
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

		
		UniformBufferTechnique::uploadMaterialData();
		UniformBufferTechnique::uploadTextureData();
 		UniformBufferTechnique::uploadTextureViewData();

		glClearColor(0.001f, 0.001f, 0.001f, 1.0f);
	}

	float fps = 0.0f;
	float cumTime = 0.0f;
	uint cumFrames = 0;

	void onUpdate(float timestep) override {
		cameraController.onUpdate(timestep);

		TextRenderer::clearScene();
		std::string content;
		for (int i = 0; i < 1000; i++) {
			content += rand() % 128 + 32;
			if ((i - 40) % 100 == 0) content += '\n';
		}
		TextRenderer::submitText(content, {1, 200});

		cumTime += timestep;
		cumFrames++;
		if (cumTime > 0.5f) {
			fps = (float)cumFrames / cumTime;
			cumFrames = 0;
			cumTime = 0;
		}

		std::string timer = std::format("FPS {:.1f}", fps);
		TextRenderer::submitText(timer, {20, 27}, 0.5);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		renderer.render(scene);
	}

	void onEvent(BaseEvent& e) override {
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseMoveEvent>(FORWARD_FN(cameraController.onMouseMoveEvent));
		dispatcher.dispatch<KeyboardButtonEvent>(FORWARD_FN(cameraController.onKeyboardButtonEvent));
	}
};

