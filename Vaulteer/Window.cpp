#include "Window.h"

Window::Window(const std::string title, unsigned const int width, unsigned const int height) : DebugLogger<Window>("WindowStuff")
{
	setup(title, width, height);
	int init = !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	{
		debug("GLAD initialization: " + std::to_string(init) + "\n","gladLoadGLLoader");
	}
}

int Window::is_running()
{
	return !glfwWindowShouldClose(window);
}

GLFWwindow* Window::getRawWindow() const
{
	return window;
}

int Window::getHeight()
{
	int height;
	glfwGetWindowSize(window, nullptr, &height);
	return height;
}

int Window::getWidth()
{
	int width;
	glfwGetWindowSize(window, &width, nullptr);
	return width;
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	DebugLogger<Window> log;
	log.debug("Viewport: width=" + std::to_string(width) + "height=" + std::to_string(height) + "\n", "glViewport");
}

void Window::setup(const std::string title, unsigned const int width, unsigned const int height)
{

	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	bool success = window != nullptr;
	debug("GLFW window initialization: " + std::to_string(success) + "\n");
	//MUST IMPLEMENT DEBUGLOGGER STOPPER THINGY
	//glfwTerminate(); <<<<<<<<<

	glfwMakeContextCurrent(window);
	debug("GLFW window context set.\n");

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	debug("GLFW window resize framebuffer callback set.\n");
}
