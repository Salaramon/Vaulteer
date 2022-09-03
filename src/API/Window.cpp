#include "vpch.h"
#include "Window.h"

Window::Window(const std::string title, unsigned const int width, unsigned const int height) : 
	OR(this, DY::V(&window), DY::N("window"))
{
	OB.add(OR);
	setup(title, width, height);
	int init = !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

int Window::is_running()
{
	return !glfwWindowShouldClose(window);
}

GLFWwindow* Window::getRawWindow() const
{
	LOG::CLAS::debug<&Window::getRawWindow>(this, &window, "raw GLFWwindow was returned");
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
	LOG::CLAS::debug<&Window::addResizeCallback>(this, &window, "window resize callback added");
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

	LOG::SPGL::debug<&framebuffer_size_callback, Window>(&window, "window was resized");
}

void Window::focus_callback(GLFWwindow* window, int focused)
{
	//if (focused == GLFW_TRUE) {
		glfwMakeContextCurrent(window);
	//}
	//else {
	//	glfwMakeContextCurrent(nullptr);
	//}

	LOG::SPGL::debug<&focus_callback, Window>(&window, "Window was focused");
}

void Window::iconify_callback(GLFWwindow* window, int iconified)
{
	//if (iconified == GLFW_FALSE) {
		glfwMakeContextCurrent(window);
	//}
	//else {
	//	glfwMakeContextCurrent(nullptr);
	//}

	LOG::SPGL::debug<&iconify_callback, Window>(&window, "Window was iconified");
}

void Window::setup(const std::string title, unsigned const int width, unsigned const int height)
{

	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	bool success = window != nullptr;
	//debug("GLFW window initialization: " + std::to_string(success) + "\n");
	//MUST IMPLEMENT DEBUGLOGGER STOPPER THINGY
	//glfwTerminate(); <<<<<<<<<

	LOG::CLAS::debug<&Window::setup>(this, &window, std::format("Window created with title {} and dimensions {}x{}", title, width, height));

	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	resizeCallbacks.emplace(window, std::vector<std::function<void(int, int)>>());

	glfwSetWindowFocusCallback(window, focus_callback);

	glfwSetWindowIconifyCallback(window, iconify_callback);
}
