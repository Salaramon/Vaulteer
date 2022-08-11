#include "vpch.h"
#include "Game.h"

Game::Game(Window& window) :
	window(&window) {}

void Game::loadResources() {
	std::vector<ModelResourceLocator> locators = {
		{ "crate", "resources/crate/crate1.obj" },
		{ "palm", "resources/palm/palm.obj", aiProcess_GenNormals | aiProcess_Triangulate | aiProcess_FlipUVs },
		//{ "plane", "resources/plane/plane_mesh.obj", aiProcess_GenNormals | aiProcess_Triangulate },
		//{ "vase", "resources/vase/untitled.obj" },
		//{ "backpack", "backpack/backpack.obj" },
		//{ "teapot", "resources/backpack/teapot.obj", aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_FlipUVs },
	};
	resourceManager.createPack(locators);
}

float randf(float from, float to) {
	return from + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (to - from)));
}

size_t Game::run() {
	//glEnable(GL_LINE_SMOOTH);

 	glfwSetInputMode(window->getRawWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Renderer
	auto rebuildGBuffer = [&](int w, int h) { 
		DeferredRenderer::rebuildGBuffer(w, h); 
		BlendingForwardRenderer::rebuildAlphaBuffer(w, h); 
	};
	window->addResizeCallback(rebuildGBuffer);

	Renderer<DeferredRenderer> opaqueRenderer;
	Renderer<BlendingForwardRenderer> transparentRenderer;

	DeferredRenderer::initialize(window->getWidth(), window->getHeight());
	BlendingForwardRenderer::initialize(window->getWidth(), window->getHeight());

	// TODO for dan: make function for printing and breaking at the same time (by message key)

	//Scenes
	DynamicScene<Camera> dynamicScene;
	//StaticScene<Model<ModelData>> staticScene;
	StaticScene<OpaqueModel, Model<LineData>> opaqueScene;
	StaticScene<TransparentModel> transparentScene;

	//Setting up cameras in the scene.
	Camera* camera = dynamicScene.addObject(Camera(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 0, 1000, 60, (float)window->getWidth() / (float)window->getHeight()));
	auto setAspectRatio = [&](int w, int h) { camera->setAspectRatio(w, h); };
	window->addResizeCallback(setAspectRatio);


	ResourcePack& pack = resourceManager.getPack(0);
	Model<ModelData> model1 = Model<ModelData>(pack.getModelByName("palm"));
	Model<ModelData> model2 = Model<ModelData>(pack.getModelByName("crate"));

	DeferredRenderer::preload(pack);

	//Add models to scene layers(s)
	std::vector<Object3D*> objects;

	std::vector<OpaqueModel> opaqueModels;

	//Generate floor
	intmax_t width = 10;
	intmax_t height = 10;
	for (intmax_t i = -(ceil(width / 2.0f)); i < (ceil(width / 2.0f)); i++) {
		for (intmax_t j = -(ceil(height / 2.0f)); j < (ceil(height / 2.0f)); j++) {
			float r = randf(1.0, 25.0);
			float x = sin(randf(0.f, M_2_PI)) * r;
			float y = cos(randf(0.f, M_2_PI)) * r;

			model1.setPosition(8 * i + x, ((float)(rand() % 8)/8), 8 * j + y);
			model1.setRotation(((float) (rand() % 360) / M_PI), glm::vec3(0, 1, 0));
			opaqueScene.addObject(std::move(OpaqueModel(model1)), model1.getBoundingSphere());
		}
	}

	model2.setPosition(0, 0, 0);
	model2.setScale(glm::vec3(5.0f));
	opaqueScene.addObject(std::move(OpaqueModel(model2)), model2.getBoundingSphere());

 	opaqueScene.finalize();

	model2.setPosition(0, 12, 10);
	transparentScene.addObject(std::move(TransparentModel(model2)), model2.getBoundingSphere());
	model2.setPosition(10, 12, 0);
	transparentScene.addObject(std::move(TransparentModel(model2)), model2.getBoundingSphere());

	transparentScene.finalize();

	//Setup variables and function calls.
	glm::vec3 worldUp = glm::vec3(0, 1, 0);

	camera->lockUp(worldUp);
	camera->setPosition(0, 10, 0);

	double vel = 0;
	glClearColor(0.00f, 0.00f, 0.00f, 1.0f);

	while (window->is_running()) {

		Event::Poll();

		float sens = 0.155f;
		float speed = 20.0f;


		camera->rotate(Event::CURSOR::X.delta() * sens / 1.35, Event::CURSOR::Y.delta() * sens, 0);
		if (Event::isKeyDown(Event::KEY::SPACE)) {
			camera->move(worldUp * (float)Event::delta() * speed);
		}
		if (Event::isKeyDown(Event::KEY::LEFT_CONTROL)) {
			camera->move(-worldUp * (float)Event::delta() * speed);
		}
		if (Event::isKeyDown(Event::KEY::W)) {
			camera->move(camera->getFront() * (float)Event::delta() * speed);
		}
		if (Event::isKeyDown(Event::KEY::S)) {
			camera->move(-camera->getFront() * (float)Event::delta() * speed);
		}
		if (Event::isKeyDown(Event::KEY::A)) {
			camera->move(camera->getRight() * (float)Event::delta() * speed);
		}
		if (Event::isKeyDown(Event::KEY::D)) {
			camera->move(-camera->getRight() * (float)Event::delta() * speed);
		}
		if (Event::isKeyDown(Event::KEY::E)) {
			camera->rotate(0, 0, (float)Event::delta() * (speed / sens));
		}
		if (Event::isKeyDown(Event::KEY::Q)) {
			camera->rotate(0, 0, -(float)Event::delta() * (speed / sens));
		}

		if (Event::isKeyPressed(Event::KEY::T)) {
			//DebugLogger<>::enableLogging();
			//std::cout << "reloading shader" << std::endl;
			DeferredRenderer::reloadShaders();

			// TODO for dan: make function for printing and breaking at the same time (by message key)
		}

		camera->apply();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		opaqueRenderer.render(dynamicScene, opaqueScene);
		transparentRenderer.render(dynamicScene, transparentScene);

		glfwSwapBuffers(window->getRawWindow());

		//DebugLogger<>::disableLogging();
	}

	return 0;
}

void Game::setWindow(Window& window) {
	Game::window = &window;
}
