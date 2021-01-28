
#define STB_IMAGE_IMPLEMENTATION

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"

int main() {

	glfwInit();

	Window window("Vaulteer", 1280, 720);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	stbi_set_flip_vertically_on_load(true);
	glEnable(GL_DEPTH_TEST);

	Shader shader("vertex.shader", "fragment.shader");
	shader.use();
	Model model("Crate/Crate1.obj");
	Camera camera(glm::vec3(0.0f, 0.0f, 6.0f));

	float deltaTime = 0, lastFrame = 0;

	while (window.is_running()) {

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

		glfwSwapBuffers(window.getRawWindow());
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

//TODO
/*
Import and modify TileMap
Event handling

Make camera and make it able to rotate around the y axis
*/

enum class KEY {
	A = 1,
	B = 2
};

enum class ACTION {
	PRESS = 1,
	RELEASE = 2
};

struct Event {

	static void Poll() {};

	struct Key {
		bool operator()(KEY key) {
			//do something
		}
	};
	struct Action {
		bool operator()(ACTION key) {
			//do something
		}
	};

	static Key Key;
	static Action Action;
};

void test() {
	
	Event::Poll();
	Event::Key == KEY::A && Event::Action == ACTION::PRESS;
}