#include "vpch.h"
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

void Window::addResizeCallback(std::function<void(int, int)> callback)
{
	resizeCallbacks.at(window).push_back(callback);
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

	GLFWwindow* currentWindow = glfwGetCurrentContext();

	auto it = resizeCallbacks.find(currentWindow);

	if (it != resizeCallbacks.end()) {
		std::vector<std::function<void(int, int)>>& callbackVector = it->second;

		for (auto& fn : callbackVector) {
			fn(width, height);
		}
	}

}

void Window::focus_callback(GLFWwindow* window, int focused)
{
	if (focused == GLFW_TRUE) {
		glfwMakeContextCurrent(window);
	}
	else {
		glfwMakeContextCurrent(nullptr);
	}

}

void Window::iconify_callback(GLFWwindow* window, int iconified)
{
	if (iconified == GLFW_FALSE) {
		glfwMakeContextCurrent(window);
	}
	else {
		glfwMakeContextCurrent(nullptr);
	}
}

void Window::setup(const std::string title, unsigned const int width, unsigned const int height)
{

	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	bool success = window != nullptr;
	debug("GLFW window initialization: " + std::to_string(success) + "\n");
	//MUST IMPLEMENT DEBUGLOGGER STOPPER THINGY
	//glfwTerminate(); <<<<<<<<<

	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	resizeCallbacks.emplace(window, std::vector<std::function<void(int, int)>>());

	glfwSetWindowFocusCallback(window, focus_callback);

	glfwSetWindowIconifyCallback(window, iconify_callback);
}
