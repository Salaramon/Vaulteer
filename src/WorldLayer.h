#pragma once
#include "API/Layer.h"
#include "API/CameraController.h"

#include "Renderer/Renderer.h"
#include "Renderer/ForwardRenderer.h"

#include "API/Application.h"
#include "Model/Resources/ResourceManager.h"
#include "Renderer/ModelRenderer.h"
#include "Renderer/TextRenderer.h"
#include "Utils/MathUtils.h"



// TODO: just a container for what's essentially test/demo data before actual game specific data is designed
// anything here can be assumed to not be integral to our current structure, but rather an example of how we use types
struct World {
	//Scenes
	inline static constexpr size_t scene_0 = 0;
	Scene<scene_0> scene;

	Camera camera;
	CameraController cameraController;

	std::vector<std::unique_ptr<Model>> loadedModels;

	std::vector<PointLight> lights;
	std::vector<Entity> lightEntities;
	
	Meshes specialCrate;
	std::vector<Meshes> lightMeshes;

	std::vector<Model*> shadowModels;
};

bool cmp_model_by_shadertype(const std::unique_ptr<Model>& a, const std::unique_ptr<Model>& b) {
    return a->meshes->at(0)->shaderType < b->meshes->at(0)->shaderType;
}


namespace WorldLayer {
	
	inline World world;

	//Renderer<DeferredRenderer, BlendingForwardRenderer, TextRenderer> renderer;
	//inline Renderer<ForwardRenderer, TextRenderer> renderer;
	inline ModelRenderer renderer;

	void onAttach(void* context) {
		Scene<World::scene_0>* scene = &world.scene;

		Window& window = Application::getWindow();

		auto setAspectRatio = [](int w, int h) {
			world.camera.propertiesCamera.aspectRatio = (float)w / h;
		};
 		Window::addResizeCallback(setAspectRatio);
		
		ResourcePack& pack = ResourceManager::getPack(0);
		
		Entity& lightEntity = world.lightEntities.emplace_back();
		DirectionalLight dirLight = {
			{glm::vec3(1.0f, 1.0f, 1.0f), 0.001f, 0.01f},
			glm::vec3(0.0f, -1.0f, -1.0f)};
		lightEntity.add<DirectionalLight>(dirLight);
		world.scene.add(lightEntity);



		// TODO it's not actually just initialization, but also setting texture state we don't expect to change in our demo... but it should be handled
		DeferredRenderer::initialize(world.camera, pack.getTextureID(), Window::getWidth(), Window::getHeight());
		BlendingForwardRenderer::initialize(world.camera, pack.getTextureID(), Window::getWidth(), Window::getHeight());
		ForwardRenderer::initialize(world.camera, pack.getTextureID());

		TextRenderer::initialize(Window::getWidth(), Window::getHeight());


		
		world.camera.enableAxisLock();
		world.camera.setLockAxis({0,1,0});

		world.camera.propertiesCamera.aspectRatio = (float)Window::getWidth()/Window::getHeight();
		world.camera.propertiesCamera.fov = 60;
		world.camera.propertiesCamera.near = 0.1f;
		world.camera.propertiesCamera.far = 1000;

		world.cameraController.setCamera(&world.camera);

		scene->activeCamera = &world.camera;
		scene->add(world.camera);


		Model& palm = *world.loadedModels.emplace_back(std::make_unique<Model>(pack.getMeshes("palm")));
		palm.setPosition(glm::vec3(0, 0, -5));
		palm.setScale(glm::vec3(0.5f));
		palm.add<Shadow>();
		world.shadowModels.push_back(&palm);

		Model& quad = *world.loadedModels.emplace_back(std::make_unique<Model>(pack.getMeshes("quad")));
		quad.setPosition({0.f, -2.f, 0.f});
		quad.setRotation(-M_PI_2, 0.f, 0.f);
		quad.setScale({100.f, 1.f, 100.f});
		
		Model& quad2 = *world.loadedModels.emplace_back(std::make_unique<Model>(pack.getMeshes("quad")));
		quad2.setPosition({0.f, 0.f, -10.f});
		quad2.setRotation(0.f, 0.f, -M_PI_2);
		quad2.setScale({100.f, 100.f, 1.f});

		int plane = 20;
		int dispersion = 2;
		
		//srand(time(nullptr));

		std::vector<glm::vec3> lightColors = {
			{1.0, 0.01, 0.01},
			{1.0, 1.0, 0.01},
			{0.01, 1.0, 0.01},
			{0.01, 1.0, 1.0},
			{0.01, 0.01, 1.0},
			{1.0, 0.01, 1.0},
		};
		
		std::vector<Material*> lightMats(6);
		
		Material copy = *MaterialLibrary::get(0);
		copy.data.colorAmbient = 0.9f;
		copy.data.matOpacity = 0.1f;

		for (int i = 0; i < lightColors.size(); i++) {
			copy.data.colorAmbient = lightColors[i];
			copy.data.matOpacity = 0.1f;
			lightMats[i] = MaterialLibrary::create(copy.data, std::format("lightMat_{}", i));
		}

		int meshCounter = 0;
		for (int y = 0; y < plane * dispersion; y += 2 * dispersion) {
			for (int x = -plane * dispersion; x < plane * dispersion; x += 2 * dispersion) {
				if (rand() % 8 == 0) {
					int light = rand() % 6;
					// TODO need consistent way to refer to resources
					world.lightMeshes.push_back({new Mesh(GeometryLibrary::get("resources/crate/crate1.obj_0"), lightMats[light])});
					Model& crate = *world.loadedModels.emplace_back(std::make_unique<Model>(world.lightMeshes[meshCounter++]));

					crate.setPosition(glm::vec3(x, y, 10.0f));
					crate.setScale(glm::vec3(0.2f));

					BaseLight base = {lightColors[light], 0.13f, 0.5f};
					Attenuation att = { 1.0f, 0.18f, 0.032f };
					PointLight& pointLight = world.lights.emplace_back(att, base, *crate.position);
					crate.add<PointLight>(pointLight);
					
					if (meshCounter >= 1) 
						goto skip;
				}
			}
		}
		skip:

		world.specialCrate = {new Mesh(*pack.getMeshes("crate")[0])};
		Model& cratei = *world.loadedModels.emplace_back(std::make_unique<Model>(world.specialCrate));
		cratei.add<Shadow>();
		world.shadowModels.push_back(&cratei);
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
		

		for (auto& model : world.loadedModels) {
			model->addRenderComponents();
			model->finalizeTransform();

			if (model->propertiesModel->instanceNumber == 0)
				scene->add(*model);
		}


		// pretty important for renderer correctness currently (using ModelRenderer). figure out what to do with it
		std::ranges::sort(world.loadedModels, cmp_model_by_shadertype);



		UniformBufferTechnique::uploadMaterialData();
		UniformBufferTechnique::uploadTextureData();
 		UniformBufferTechnique::uploadTextureViewData();
		UniformBufferTechnique::uploadCameraProjection(scene->getActiveCamera().projectionMatrix());

		DeferredRenderer::buildLights(*scene);
		DeferredRenderer::data.shadowModels = &world.shadowModels;

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

	void onUpdate(void* context, float timestep) {
		Scene<World::scene_0>* scene = &world.scene;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		world.cameraController.onUpdate(timestep);

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

		glm::mat4 view = scene->getActiveCamera().viewMatrix();
		UniformBufferTechnique::uploadCameraView(view);

		// -- render --
		for (auto& model : world.loadedModels) {
			renderer.render(*model);
		}
		renderer.endFrame();
		renderer.currentType = ShaderType::None;
		TextRenderer::flush();
		// -- end render --

		cumDrawCalls += renderer.getNumDrawCalls();
		renderer.resetStats();

		TextRenderer::clearScene();
	}

	bool onKeyboardButtonEvent(KeyboardButtonEvent& e) {
		return true;
	}

	void onEvent(void* context, BaseEvent& e) {
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseMoveEvent>(FORWARD_FN(world.cameraController.onMouseMoveEvent));
		dispatcher.dispatch<KeyboardButtonEvent>(FORWARD_FN(world.cameraController.onKeyboardButtonEvent));
		dispatcher.dispatch<KeyboardButtonEvent>(FORWARD_FN(onKeyboardButtonEvent));
	}

	LayerFunctions worldFunctions {
		.context = &world,
		.onAttach = &onAttach,
		.onEvent = &onEvent,
		.onUpdate = &onUpdate 
	};
};

