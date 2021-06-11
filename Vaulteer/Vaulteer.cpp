
#define STB_IMAGE_IMPLEMENTATION

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <future>

#include "Window.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "ShaderCode.h"

#include "Event.h"
#include "GBuffer.h"


void handleEvents(Camera& camera) {
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
}

Shader buildShader() {
	shr::ShaderCode vertexCode;
	vertexCode.bind();
	/*
	shr::LocIn<shr::vec3> VertexCoords_modelspace;
	shr::LocIn<shr::vec3> VertexNormal_modelspace;
	shr::LocIn<shr::vec2> aTexCoords;

	shr::Output<shr::vec3> Position_worldspace;
	shr::Output<shr::vec3> Normal_cameraspace;
	shr::Output<shr::vec3> EyeDirection_cameraspace;
	shr::Output<shr::vec3> LightDirection_cameraspace;
	shr::Output<shr::vec2> TexCoords;

	shr::Uniform<shr::mat4> model;
	shr::Uniform<shr::mat4> view;
	shr::Uniform<shr::mat4> projection;
	shr::Uniform<shr::vec3> LightPosition_worldspace;


	TexCoords = aTexCoords;
	shr::gl_Position = projection * view * model * shr::Vec4(VertexCoords_modelspace, 1.0f);

	Position_worldspace = (model * shr::Vec4(VertexCoords_modelspace, 1.0f)).xyz();

	shr::vec3 vertexPosition_cameraspace = (view * model * shr::Vec4(VertexCoords_modelspace, 1.0f)).xyz();
	EyeDirection_cameraspace = shr::Vec3(0.0f, 0.0f, 0.0f) - vertexPosition_cameraspace;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	shr::vec3 LightPosition_cameraspace = (view * shr::Vec4(LightPosition_worldspace, 1.0f)).xyz();
	LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

	// Normal of the the vertex, in camera space
	Normal_cameraspace = (view * model * shr::Vec4(VertexCoords_modelspace, 0)).xyz(); // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
	*/

	vertexCode.unbind();
	std::cout << vertexCode.getCode() << std::endl << " -- " << std::endl;

	shr::ShaderCode fragmentCode;
	fragmentCode.bind();

	/*
	vec3 LightColor = vec3(1, 1, 1);
	float LightPower = 10.0f;

	// Material properties
	vec3 MaterialDiffuseColor = vec3(1, 1, 1);
	vec3 MaterialAmbientColor = vec3(0.04, 0.04, 0.04) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3, 0.3, 0.3);

	// Distance to the light
	float distance = length(LightPosition_worldspace - Position_worldspace);

	// Normal of the computed fragment, in camera space
	vec3 n = normalize(Normal_cameraspace);
	// Direction of the light (from the fragment to the light)
	vec3 l = normalize(LightDirection_cameraspace);
	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp(dot(n, l), 0, 1);

	// Eye vector (towards the camera)
	vec3 E = normalize(EyeDirection_cameraspace);
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-l, n);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp(dot(E, R), 0, 1);

	FragColor = MaterialAmbientColor +
		MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance * distance) +
		MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha, 6) / (distance * distance);
	*/

	fragmentCode.unbind();
	std::cout << fragmentCode.getCode() << std::endl;

	return Shader(vertexCode, fragmentCode);
}


int main() {

	glfwInit();

	Window window("Vaulteer", 1280, 720);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	/*
	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
	};

	unsigned int quadVAO, quadVBO;
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	*/


	stbi_set_flip_vertically_on_load(true);
	glEnable(GL_DEPTH_TEST);

	// initialize shader
	Shader shader = Shader("g_pass_vertex.shader", "g_pass_frag.shader");
	shader.use();

	const int WINDOW_WIDTH = 1280, WINDOW_HEIGHT = 720;

	GBuffer gbuffer = GBuffer();
	gbuffer.Init(WINDOW_WIDTH, WINDOW_HEIGHT);

	Model model("teapot.obj");
	Camera camera(glm::vec3(0.0f, 0.0f, 15.0f), 0.0f,0.0f,0.0f);

	float deltaTime = 0, lastFrame = 0;

	Event::AddEventHandlingForWindow(&window);
	glfwSetInputMode(window.getRawWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	std::cout.precision(8);


	while (window.is_running()) {

		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// geometry pass
		// ------
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//gbuffer.BindForWriting();
		shader.use();

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)1280 / (float)720, 0.1f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		
		// render the loaded model
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		modelMatrix = glm::rotate(modelMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));	// it's a bit too big for our scene, so scale it down
		shader.setMatrix("model", modelMatrix);
		shader.setMatrix("mvp", projection * view * modelMatrix);
		model.draw(shader);


		/* // from learnopengl - generates texture to use when ?
		unsigned int texColorBuffer;
		glGenTextures(1, &texColorBuffer);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		// attach it to currently bound framebuffer object
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);
		*/


		// blit entire gbuffer content to framebuffer
		// ------

		gbuffer.BindForReading();
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		GLsizei HalfWidth = (GLsizei)(WINDOW_WIDTH / 2.0f);
		GLsizei HalfHeight = (GLsizei)(WINDOW_HEIGHT / 2.0f);

		gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
		glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
			0, 0, HalfWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
		glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
			0, HalfHeight, HalfWidth, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
		glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
			HalfWidth, HalfHeight, WINDOW_WIDTH, WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		gbuffer.SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
		glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
			HalfWidth, 0, WINDOW_WIDTH, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
		

		while (GLenum error = glGetError()) {
			printf("OpenGL error %d\n", error);
		}

		handleEvents(camera);

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

