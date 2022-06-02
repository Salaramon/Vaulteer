#include "vpch.h"
#include "Game.h"

Game::Game(Window& window) :
	window(&window) {}

void Game::loadResources() {
	std::vector<ModelResourceLocator> locators = {
		{ "crate", "resources/crate/crate1.obj" },
		//{ "backpack", "backpack/backpack.obj" },
		{ "teapot", "resources/backpack/teapot.obj", aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_FlipUVs },
	};
	resourceManager.createPack(locators);

}

size_t Game::run() {
	//glEnable(GL_LINE_SMOOTH);

	glfwSetInputMode(window->getRawWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Renderer
	auto rebuildGBuffer = [&](int w, int h) { DeferredRenderer::rebuildGBuffer(w, h); };
	window->addResizeCallback(rebuildGBuffer);
	Renderer<DeferredRenderer> renderer;
	DeferredRenderer::initialize(window->getWidth(), window->getHeight());

	// TODO for dan: make function for printing and breaking at the same time (by message key)
	DebugLogger<>::setClassAccessLimit("Shader", 10);
	DebugLogger<>::printClass("Shader");
	DebugLogger<>::disableLogging();

	//Scenes
	DynamicScene<Camera> dynamicScene;
	//StaticScene<Model<ModelData>> staticScene;
	StaticScene<Model<ModelData>, Model<LineData>> staticScene;

	//Setting up cameras in the scene.
	Camera* camera = dynamicScene.addObject(Camera(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 0, 1000, 60, (float)window->getWidth() / (float)window->getHeight()));
	auto setAspectRatio = [&](int w, int h) { camera->setAspectRatio(w, h); };
	window->addResizeCallback(setAspectRatio);


	//window->addReziseCallback(camera->onResize);

	ResourcePack& pack = resourceManager.getPack(0);
	Model<ModelData> crate = Model<ModelData>(pack.getModelByName("crate"));
	Model<ModelData> backpack = Model<ModelData>(pack.getModelByName("teapot"));

	DeferredRenderer::preload(pack);

	//Add models to scene layers(s)
	std::vector<Object3D*> objects;

	std::vector<Model<ModelData>*> modelsObjects;

	//Generate floor
	intmax_t width = 32;
	intmax_t height = 32;
	for (intmax_t i = -(width / 2); i < (width / 2); i++) {
		for (intmax_t j = -(height / 2); j < (height / 2); j++) {
			crate.setPosition(4 * i, ((float)(rand() % 8)/2)-4, 4 * j);
			staticScene.addObject(std::move(crate), crate.getBoundingSphere());
		}
	}

	backpack.setPosition(0, 10, -10);
	staticScene.addObject(std::move(backpack), backpack.getBoundingSphere());

	staticScene.finalize();


	//Setup variables and function calls.
	glm::vec3 worldUp = glm::vec3(0, 1, 0);

	camera->lockUp(worldUp);
	camera->setPosition(0, 10, 0);

	double vel = 0;

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
			DebugLogger<>::enableLogging();
			std::cout << "reloading shader" << std::endl;
			DeferredRenderer::reloadShaders();

			// TODO for dan: make function for printing and breaking at the same time (by message key)
			DebugLogger<>::setClassAccessLimit("Shader", 10);
			DebugLogger<>::printClass("Shader");
			DebugLogger<>::disableLogging();
		}

		camera->apply();

		glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//renderer.add<Renderers, renderer2>(scene);
		//...


		renderer.render(dynamicScene, staticScene);

		glfwSwapBuffers(window->getRawWindow());

		DebugLogger<>::disableLogging();
	}

	//DebugLogger<>::setDefaultClassMessageLimit(0);
	//DebugLogger<>::setClassMessageLimit("FUNCTION", 99999);
	//DebugLogger<>::setMessageNameMessageLimit(ObjectAlias::OpenGLMessage, 99999);


	//DebugLogger<>::setDefaultClassMessageLimit(0);
	//DebugLogger<>::setClassMessageLimit("Shader", 99999);

	DebugLogger<>::printOrderedByClass();

	return 0;
}

void Game::setWindow(Window& window) {
	Game::window = &window;
}
