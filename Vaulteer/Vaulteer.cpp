
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

#include "ShaderCode.h"
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

	
	shr::ShaderCode vertexShader;
	
	vertexShader.bind();

	shr::Vec2<shr::Float> wer;
	shr::Vec4<shr::Float> wow;
	shr::Vec4<shr::Float> wow2;
	shr::Vec4<shr::Float> wow3;
	shr::Vec4<shr::Float> wow4;

	wow = wow2 + wow3 * wow4;
	wer = shr::vec2(1.543f, 56.5654f) * wow2.xx;
	wer = shr::vec2(wer, 5.23f);
	wer = shr::vec3(343.54f, wer);
	wer = shr::mat3(2345.0f, 3455.0f, 3453.0f);

	//shr::Mat3<shr::Float> test;
	//wer(34, 65);
	//test(4553, 435, 345, 56, 5, 34, 53, 4, 56, 45);

	/*

	shr::LocIn<shr::vec3> defVSLocInPos;
	shr::LocIn<shr::vec3> defVSLocInNormal;
	shr::LocIn<shr::vec2> defVSLocInTexCoord;

	shr::Output<shr::vec2> defVSOutTexCoord;

	shr::Uniform<shr::mat4> defVSUniView;
	shr::Uniform<shr::mat4> defVSUniProj;
	shr::Uniform<shr::mat4> defVSUniModel;

	
	
	defVSOutTexCoord = defVSLocInTexCoord;
	shr::gl_Position = defVSUniProj * defVSUniView * defVSUniModel * shr::Vec4(defVSLocInPos, 1.0);
	*/
	vertexShader.unbind();
	
	std::cout << vertexShader.getCode() << std::endl;

	shr::ShaderCode fragmentShader;

	fragmentShader.bind();
	/*
	shr::Output<shr::vec4> defFSColor;

	shr::Input<shr::vec2> defFSTexture(defVSOutTexCoord);

	shr::Uniform<shr::sampler2D> defFSUniDiffuse;

	defFSColor = shr::texture(defFSUniDiffuse, defFSTexture);
	*/
	fragmentShader.unbind();

	

	/*
	shr::Struct<types...> myStruct;
	myStruct = a, b, a, ...;
	myStruct = {a, b, c, ...};
	*/
	/*
	shr::ShaderCode test;
	test.bind();
	shr::LocIn<shr::vec3> VertexCoords_modelspace;
	shr::LocIn<shr::vec3> VertexNormal_modelspace;
	shr::LocIn<shr::vec2> aTexCoords;

	shr::Uniform<shr::mat4> modela;
	shr::Uniform<shr::mat4> viewa;
	shr::Uniform<shr::mat4> projectiona;

	shr::Output<shr::vec3> EyeDirection_cameraspace;
	shr::Output<shr::vec3> Position_worldspace;

	shr::gl_Position = projectiona * viewa * modela * shr::Vec4(VertexCoords_modelspace, 1.0f).xyz();

	Position_worldspace = (modela * shr::Vec4(VertexCoords_modelspace, 1.0));

	shr::vec3 vertexPosition_cameraspace; vertexPosition_cameraspace  = (viewa * modela * shr::Vec4(VertexCoords_modelspace, 1.0f)).xyz();
	EyeDirection_cameraspace = shr::Vec3(0.0f, 0.0f, 0.0f) - vertexPosition_cameraspace;
	
	test.unbind();
	*/
	
	/*
		Vec4(Uniform, float)
		xyz
		Vec3(float,float,float)
	*/

	//std::cout << test.getCode() << std::endl;

	Shader shader(vertexShader, fragmentShader);
	shader.use();

	Model model("Crate/Crate1.obj");
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
		shader.setMatrix("projection", projection);
		shader.setMatrix("view", view);
		
		glm::mat4 modelMatrix = glm::mat4(1.0f);

		// render the loaded model
		shader.setMatrix("model", modelMatrix);
		//model.draw(shader);
		
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

