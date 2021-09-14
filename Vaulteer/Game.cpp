#include "Game.h"

Game::Game(Window& window) : 
	window(&window)
{
}

void Game::loadResources()
{

	models.emplace(std::make_pair<std::string, ModelData>("Crate", {"Crate/Crate1.obj", "Crate" }));
}

size_t Game::run()
{
	
	glfwSetInputMode(window->getRawWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	Scene gameScene;
	
	std::vector<Model*> models;

	//Generate floor
	intmax_t width = 14;
	intmax_t height = 14;
	for (intmax_t i = -7; i < width; i++) {
		for (intmax_t j = -7; j < height; j++) {
			models.push_back(gameScene.addModel(Model(modelByName("Crate"))));
			models.back()->setPosition(2*i, 0, 2*j);
		}
	}

	Shader shader(Binder::file_names::lightsource_vertex, GL_VERTEX_SHADER, Binder::file_names::lightsource_frag, GL_FRAGMENT_SHADER);

	RenderPass worldPass(shader);
	Renderer renderer;
	renderer.addRenderPass(worldPass);
	glm::vec3 worldUp = glm::vec3(0, 1, 0);

	Camera* camera = gameScene.addCamera(Camera(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 0, 1000, 60, (float)window->getWidth() / (float)window->getHeight()));
	camera->lockUp(worldUp);
	camera->setPosition(0, 10, 0);
	
	gameScene.setActiveCamera(camera);

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

		/*
		glm::vec3 cameraPos = camera.getPosition();
		
		if (vel < 52) {
			vel = vel + (9.81 * Event::delta());
		}
		
		cameraPos.y = cameraPos.y - vel;
		camera.setPosition(cameraPos.x, cameraPos.y, cameraPos.z);

		if (cameraPos.y < 4) {
			camera.setPosition(cameraPos.x, 4, cameraPos.z);
			vel = 0;
		}
		*/
		camera->apply();

		glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		
		renderer.render(gameScene);

		glfwSwapBuffers(window->getRawWindow());
	}
	
	return 0;
}

void Game::setWindow(Window& window)
{
	Game::window = &window;
}

ModelData& Game::modelByName(std::string name)
{
	auto it = models.find(name);
	if (it == models.end()) {
		debug("Model name not found.", MessageAlias::CriticalError);
	}
	else {
		return it->second;
	}
}
