
#define STB_IMAGE_IMPLEMENTATION

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <future>

#include "Window.h"
#include "Model.h"
#include "Camera.h"
#include "Event.h"

#include "GLSLCPPBinder.h"

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexAttribute.h"

#include "Shader.h"


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

	Texture::uniformTextureTypes.emplace(aiTextureType_DIFFUSE, Binder::fragment::uniforms::diffuse1);

	Shader shader("vertex.shader", "fragment.shader");
	shader.use();

	Model model("Crate/Crate1.obj","Crate");
	model.setShaderContext(&shader);
	Camera camera(glm::vec3(0.0f, 0.0f, 15.0f), 0.0f,0.0f,0.0f);

	float deltaTime = 0, lastFrame = 0;

	Event::AddEventHandlingForWindow(&window);

	std::cout.precision(8);
	glfwSetInputMode(window.getRawWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);


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
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)1280 / (float)720, 0.1f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		shader.setUniform(Binder::vertex::uniforms::projection, 1, GL_FALSE, projection);
		shader.setUniform(Binder::vertex::uniforms::view, 1, GL_FALSE, view);;
		
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));

		// render the loaded model
		shader.setUniform(Binder::vertex::uniforms::model, 1, GL_FALSE, modelMatrix);
		model.draw();
		
		float speed = 5.0f;
		Event::Poll();
		//std::cout << (Event::Count << (Event::CursorX > 540 && Event::CursorX < 740 && Event::CursorY > 260 && Event::CursorY < 460)) << std::endl;
		double_t zDifference = 0.0f;
		if (Event::Check << (Event::Key == Event::KEY::Q && Event::State == Event::STATE::DOWN)) {
			zDifference = (float)Event::delta() * speed * 100.0f;
		}
		if (Event::Check << (Event::Key == Event::KEY::E && Event::State == Event::STATE::DOWN)) {
			zDifference = -(float)Event::delta() * speed * 100.0f;
		}

		Event::CursorEvents events = (Event::CursorEventList << (Event::CursorX > -9999999 && Event::CursorX < 9999999 && Event::CursorY > -9999999 && Event::CursorY < 9999999));
		if (!events.empty()) {
			double_t xDifference = events.back().CursorX - events.front().CursorX;
			double_t yDifference = events.back().CursorY - events.front().CursorY;
			camera.rotate(0.1f * xDifference, 0.1f * yDifference, 0.1f * zDifference);
			//std::cout << camera.getOrientation() << std::endl;
		}

		
		if (Event::Check << (Event::Key == Event::KEY::W && Event::State == Event::STATE::DOWN)) {
			camera.move(camera.getFront() * (float)Event::delta() * speed);
		}
		if (Event::Check << (Event::Key == Event::KEY::S && Event::State == Event::STATE::DOWN)) {
			camera.move(-camera.getFront() * (float)Event::delta() * speed);
		}
		if (Event::Check << (Event::Key == Event::KEY::A && Event::State == Event::STATE::DOWN)) {
			camera.move(camera.getRight() * (float)Event::delta() * speed);
		}
		if (Event::Check << (Event::Key == Event::KEY::D && Event::State == Event::STATE::DOWN)) {
			camera.move(-camera.getRight() * (float)Event::delta() * speed);
		}
		if (Event::Check << (Event::Key == Event::KEY::LEFT_SHIFT && Event::State == Event::STATE::DOWN)) {
			camera.move(camera.getUp() * (float)Event::delta() * speed);
		}
		if (Event::Check << (Event::Key == Event::KEY::SPACE && Event::State == Event::STATE::DOWN)) {
			camera.move(-camera.getUp() * (float)Event::delta() * speed);
		}


		glfwSwapBuffers(window.getRawWindow());
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

