#include "vpch.h"
#include "Game.h"

Game::Game(Window& window) :
	window(&window) {}

void Game::loadResources() {
	std::vector<ModelResourceLocator> locators = {
		{ "crate", "crate/crate1.obj" },
		//{ "backpack", "backpack/backpack.obj" },
		{ "teapot", "backpack/teapot.obj", aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_FlipUVs },
	};
	resourceManager.createPack(locators);

	/*
	lines.emplace(std::make_pair<std::string, std::unique_ptr<LineData>>("line", std::make_unique<LineData>(
		glm::vec3(5, 5, 5), glm::vec3(10, 5, 5),
		glm::vec3(5, 5, 5), glm::vec3(10, 10, 5),
		glm::vec3(10, 5, 5), glm::vec3(10, 10, 5),

		glm::vec3(5, 5, 5), glm::vec3(5, 5, 10),
		glm::vec3(5, 5, 5),  glm::vec3(5, 10, 10),
		glm::vec3(5, 5, 10), glm::vec3(5, 10, 10)
	)));
	models.emplace(std::make_pair<std::string, std::unique_ptr<ModelData>>("chaos1", std::make_unique<ModelData>(
		1, 1,
		std::vector<glm::vec4>({
			glm::vec4(0,0,1,1)
		}),
		std::vector<Vertex>({
			glm::vec3(5, 5, 5), glm::vec3(10, 5, 5), glm::vec3(10, 10, 5),
			glm::vec3(5, 5, 5), glm::vec3(5, 5, 10), glm::vec3(5, 10, 10)
		}
	))));

	models.emplace(std::make_pair<std::string, std::unique_ptr<ModelData>>("chaos2", std::make_unique<ModelData>(
		1, 1,
		std::vector<glm::vec4>({
			glm::vec4(1, 1, 1, 1)
		}),
		std::vector<Vertex>({
			glm::vec3(5, 5, 5), glm::vec3(10, 5, 5), glm::vec3(10, 10, 5),
			glm::vec3(5, 5, 5), glm::vec3(5, 5, 10), glm::vec3(5, 10, 10)
	}))));

	std::vector<Point> spherePoints;
	const size_t resolution = 256;
	const size_t curves = 256;

	for (size_t i = 0; i < curves; i++) {
		for (size_t j = 0; j < resolution; j++) {
			spherePoints.push_back(Point(
				std::cos(glm::radians((360.0 * static_cast<double>(j)) / resolution))*std::sin(glm::radians((360.0 * static_cast<double>(i)) / curves)),
				std::cos(glm::radians((360.0 * static_cast<double>(j)) / resolution))* std::cos(glm::radians((360.0 * static_cast<double>(i)) / curves)),
				std::sin(glm::radians((360.0 * static_cast<double>(j)) / resolution))
			));
		}
	}

	lines.emplace(std::make_pair<std::string, std::unique_ptr<LineData>>("sphere", std::make_unique<LineData>(
		spherePoints
	)));
	*/

}

size_t Game::run() {
	//glEnable(GL_LINE_SMOOTH);

	glfwSetInputMode(window->getRawWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Renderer
	Renderer<DeferredRenderer> renderer;
	renderer.initialize(window->getWidth(), window->getHeight());

	// TODO for dan: make function for printing and breaking at the same time (by message key)
	DebugLogger<>::setClassAccessLimit("Shader", 10);
	DebugLogger<>::printClass("Shader");
	DebugLogger<>::disableLogging();

	//Scenes
	Renderer<DeferredRenderer>::Scene scene;
	Renderer<DeferredRenderer>::Scene dynamicScene;

	//Setting up cameras in the scene.
	Camera* camera = scene.addObject(Camera(glm::vec3(0, -10, 0), glm::vec3(0, 1, 0), 0, 1000, 60, (float)window->getWidth() / (float)window->getHeight()));
	//Select the active camera.
	//scene.setActiveCamera(camera);

	//Set up render passe(s) for each render strata.
	// none

	ResourcePack& pack = resourceManager.getPack(0);
	Model<ModelData> crate = Model<ModelData>(pack.getModelByName("crate"));
	Model<ModelData> backpack = Model<ModelData>(pack.getModelByName("teapot"));

	renderer.preload(pack);

	//Add models to scene layers(s)
	std::vector<Object3D*> objects;

	objects.push_back(scene.addObject(std::move(backpack)));
	objects.back()->setPosition(0, 5, 0);

	//Generate floor
	intmax_t width = 16;
	intmax_t height = width;
	for (intmax_t i = -(width / 2); i < width; i++) {
		for (intmax_t j = -(height / 2); j < height; j++) {
			objects.push_back(scene.addObject(std::move(crate)));
			objects.back()->setPosition(2 * i, rand() % 4, 2 * j);
			//objects.back()->setScale(0.4, 0.4, 0.4);
		}
	}


	objects.push_back(scene.addObject(std::move(crate)));
	objects.back()->setPosition(1, 10, 1);
	objects.back()->setScale(0.6, 0.6, 0.6);

	//objects.back()->setRotation(glm::radians(90.0f), glm::vec3(1, 0, 0));
	//objects.back()->setScale(1000, 1000, 1000);
	//models.push_back(meshLayer->addModel(modelByName("chaos1")));
	//models.push_back(meshLayer->addModel(modelByName("chaos2")));
	//models.back()->setPolygonMode(Model::Polygon::Line);
	//models.back()->setPolygonLineWidth(5);

	//models.push_back(lineLayer->addModel(modelByName("line")));
	//models.back()->setPolygonLineWidth(5);

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
			renderer.reload();

			// TODO for dan: make function for printing and breaking at the same time (by message key)
			DebugLogger<>::setClassAccessLimit("Shader", 10);
			DebugLogger<>::printClass("Shader");
			DebugLogger<>::disableLogging();
		}

		camera->apply();

		glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		renderer.render(scene, dynamicScene);

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
