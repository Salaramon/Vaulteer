#include "Game.h"

Game::Game() //:
	//window(std::move(Window("Vaulteer", 1280, 720))),
	//shader("vertex.shader", "fragment.shader")
{
	stbi_set_flip_vertically_on_load(true);
}

void Game::run()
{
	window = new Window("Vaulteer", 1280, 720);
	//glfwMakeContextCurrent(window->getRawWindow());

	
	glEnable(GL_DEPTH_TEST);
	
	Shader shader("vertex.shader", "fragment.shader");
	Model model("Crate/Crate1.obj");
	Camera camera(glm::vec3(0.0f, 0.0f, 6.0f));

	shader.use();
	//glfwMakeContextCurrent(NULL);
	while (window->is_running()) {
		done = true;
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// render
		// ------
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		shader.use();

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)1280 / (float)720, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMatrix("projection", projection);
		shader.setMatrix("view", view);

		// render the loaded model
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 1.0f, 0.0f)); // translate it down so it's at the center of the scene
		modelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0, 1.0, 0.0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		shader.setMatrix("model", modelMatrix);
		model.draw(shader);
		if (mainContext == nullptr) {
			mainContext = glfwGetCurrentContext();
		}
		glfwMakeContextCurrent(window->getRawWindow());
		glfwSwapBuffers(window->getRawWindow());
		glfwMakeContextCurrent(mainContext);
	}

	glfwTerminate();
	delete window;
}
