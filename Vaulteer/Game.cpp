#include "Game.h"

#include "Scene.h"

Game::Game(Window& window) : 
	window(&window)
{
}

void Game::loadResources()
{
	models.emplace(std::make_pair<std::string, std::unique_ptr<ModelData>>("crate", std::make_unique<ModelData>( "Crate/Crate1.obj", "Crate" )));
	models.emplace(std::make_pair<std::string, std::unique_ptr<ModelData>>("backpack", std::make_unique<ModelData>("backpack/backpack.obj", "backpack" )));
	models.emplace(std::make_pair<std::string, std::unique_ptr<LineData>>("line", std::make_unique<LineData>(
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
}

size_t Game::run()
{
	
	//glEnable(GL_LINE_SMOOTH);

	glfwSetInputMode(window->getRawWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Renderer

	//Shaders
	Shader meshShader(Binder::file_names::forward_vertex, GL_VERTEX_SHADER, Binder::file_names::forward_frag, GL_FRAGMENT_SHADER);
	Shader lineShader(Binder::file_names::line_vertex, GL_VERTEX_SHADER, Binder::file_names::line_frag, GL_FRAGMENT_SHADER);

	//Scenes
	//Scene scene;
	Scene<Camera> scene;

	//RenderStratas


	//Setting up cameras in the scene.
	//Camera* camera = scene.addCamera(Camera(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 0, 1000, 60, (float)window->getWidth() / (float)window->getHeight()));
	Camera cam = Camera(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 0, 1000, 60, (float)window->getWidth() / (float)window->getHeight());
	Camera* camera = &cam;
	
	//Select the active camera.
	//scene.setActiveCamera(camera);

	//Set up render passe(s) for each render strata.


	
	
	//Add models to scene layers(s)
	std::vector<Model*> models;

	//Generate floor
	intmax_t width = 8;
	intmax_t height = 8;
	for (intmax_t i = -(width/2); i < width; i++) {
		for (intmax_t j = -(height/2); j < height; j++) {
			//models.push_back(meshLayer->addModel(modelByName("crate")));
			models.back()->setPosition(2*i, 0, 2*j);
		}
	}

	//models.push_back(lineLayer->addModel(modelByName("line")));
	//models.push_back(meshLayer->addModel(modelByName("chaos1")));
	//models.push_back(meshLayer->addModel(modelByName("chaos2")));
	//models.back()->setPolygonMode(Model::Polygon::Line);
	//models.back()->setPolygonLineWidth(5);

	//models.push_back(lineLayer->addModel(modelByName("line")));
	//models.back()->setPolygonLineWidth(5);

	//Setup variables and function calls.
	glm::vec3 worldUp = glm::vec3(0, 1, 0);

	//camera->lockUp(worldUp);
	//camera->setPosition(0, 10, 0);

	double_t vel = 0;

	while (window->is_running()) {

		Event::Poll();

		float sens = 0.155f;
		float speed = 20.0f;

		
		camera->rotate(Event::CURSOR::X.delta() * sens / 1.35, Event::CURSOR::Y.delta() * sens, 0);
		if (Event::KEY::LEFT_SHIFT >> Event::STATE::DOWN) {
			camera->move(worldUp * (float)Event::delta() * speed);
		}
		if (Event::KEY::LEFT_CONTROL >> Event::STATE::DOWN) {
			camera->move(-worldUp * (float)Event::delta() * speed);
		}
		if (Event::KEY::W >> Event::STATE::DOWN) {
			camera->move(camera->getFront() * (float)Event::delta() * speed);
		}
		if (Event::KEY::S >> Event::STATE::DOWN) {
			camera->move(-camera->getFront() * (float)Event::delta() * speed);
		}
		if (Event::KEY::A >> Event::STATE::DOWN) {
			camera->move(camera->getRight() * (float)Event::delta() * speed);
		}
		if (Event::KEY::D >> Event::STATE::DOWN) {
			camera->move(-camera->getRight() * (float)Event::delta() * speed);
		}
		if (Event::KEY::Q >> Event::STATE::DOWN) {
			camera->rotate(0, 0, (float)Event::delta() * (speed / sens));
		}
		if (Event::KEY::E >> Event::STATE::DOWN) {
			camera->rotate(0, 0, -(float)Event::delta() * (speed / sens));
		}

		if (Event::KEY::T >> Event::ACTION::PRESS) {
			if (true) {
				
			}
			else {
				
			}
			
		}

		camera->apply();

		glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		
		//renderer.render(scene);

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

void Game::setWindow(Window& window)
{
	Game::window = &window;
}

Model Game::modelByName(std::string name)
{
	auto it = models.find(name);
	if (it == models.end()) {
		debug("Model name not found.", MessageAlias::CriticalError);
	}
	else {
		return Model(*it->second);
	}
}
