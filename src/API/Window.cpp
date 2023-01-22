#include "vpch.h"
#include "Window.h"

Window::Window(const std::string& title, unsigned const int width, unsigned const int height) : 
		OR(this, DY::V(&window), DY::N("window")) {
	OB.add(OR);
	setup(title, width, height);
	int init = !gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
}

Window::~Window() {
	if (--GLFWWindowCount == 0) {
		glfwTerminate();
	}
}


int Window::isRunning() {
	return !glfwWindowShouldClose(window);
}

bool Window::isFocused() {
	return focused;
}

GLFWwindow* Window::getRawWindow() {
	LOG::SPGL::debug<&Window::getRawWindow, Window>(&window, "raw GLFWwindow was returned");
	return window;
}

int Window::getHeight() {
	int height;
	glfwGetWindowSize(window, nullptr, &height);
	return height;
}

int Window::getWidth() {
	int width;
	glfwGetWindowSize(window, &width, nullptr);
	return width;
}

bool Window::onWindowCloseEvent(const WindowCloseEvent& e) {
	LOG::SPGL::debug<&onWindowCloseEvent, Window>(&window, "Window closed");
	return true;
}

bool Window::onWindowFocusEvent(const WindowFocusEvent& e) {
	focused = e.focused;

	LOG::SPGL::debug<&onWindowFocusEvent, Window>(&window, "Window was focused");
	return true;
}

bool Window::onWindowResizeEvent(const WindowResizeEvent& e) {
	glViewport(0, 0, e.width, e.height);

	GLFWwindow* currentWindow = glfwGetCurrentContext();

	if (auto it = resizeCallbacks.find(currentWindow); it != resizeCallbacks.end()) {
		std::vector<std::function<void(int, int)>>& callbackVector = it->second;

		for (auto& fn : callbackVector) {
			fn(e.width, e.height);
		}
	}

	LOG::SPGL::debug<&onWindowResizeEvent, Window>(&window, "Window was resized");
	return true;
}


void Window::addResizeCallback(std::function<void(int, int)> callback) {
	resizeCallbacks.at(window).push_back(callback);
	LOG::SPGL::debug<&Window::addResizeCallback, Window>( "Window resize callback added");
}

void Window::setup(const std::string& title, const int width, const int height) {
	if (GLFWWindowCount++ == 0) {
		int success = glfwInit();
		assert(success, "GLFW could not be initialized!");
	}

	//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	bool success = window != nullptr;

	LOG::CLAS::debug<&Window::setup>(this, &window, DY::std_format("Window created with title {} and dimensions {}x{}", title, width, height));

	glfwMakeContextCurrent(window);

	resizeCallbacks.emplace(window, std::vector<std::function<void(int, int)>>());
}

bool Window::onUpdate() {
	glfwSwapBuffers(window);
	return isRunning();
}
