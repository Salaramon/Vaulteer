#include "vpch.h"
#include "WorldLayer.h"

#include "API/Application.h"
#include "Model/Resources/ResourceManager.h"
#include "Utils/MathUtils.h"


void WorldLayer::onAttach() {
	Window& window = Application::getWindow();

	DeferredRenderer::initialize(window.getWidth(), window.getHeight());
	BlendingForwardRenderer::initialize(window.getWidth(), window.getHeight());
	
	//Setting up cameras in the scene.
	Camera* camera = dynamicScene.addObject(Camera(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 0, 1000, 60, (float)window.getWidth() / (float)window.getHeight()));
	cameraController.setCamera(camera);

	auto setAspectRatio = [camera](int w, int h) {
		camera->setAspectRatio(w, h);
	};
	window.addResizeCallback(setAspectRatio);


	ResourcePack& pack = ResourceManager::getPack(0);
	Model<ModelData> model1 = Model<ModelData>(pack.getModelByName("palm"));
	Model<ModelData> model2 = Model<ModelData>(pack.getModelByName("crate"));

	DeferredRenderer::preload(pack);

	//Add models to scene layers(s)
	std::vector<Object3D*> objects;

	std::vector<OpaqueModel> opaqueModels;

	//Generate trees how about you kill yourself
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

	//Setup variables and function calls.
	camera->setLockedUp(true);
	camera->setPosition(0, 10, 0);

	glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
}

void WorldLayer::onUpdate(float timestep) {

	cameraController.onUpdate(timestep);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	opaqueRenderer.render(dynamicScene, opaqueScene);
	transparentRenderer.render(dynamicScene, transparentScene);
}

void WorldLayer::onEvent(BaseEvent& e) {

	EventDispatcher dispatcher(e);
	dispatcher.dispatch<MouseMoveEvent>(FORWARD_FN(cameraController.onMouseMoveEvent));
	dispatcher.dispatch<KeyboardButtonEvent>(FORWARD_FN(cameraController.onKeyboardButtonEvent));
}
