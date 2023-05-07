#include "vpch.h"
#include "Window.h"

Window::Window(const std::string& title, unsigned const int width, unsigned const int height) {
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
	// todo why is this handled here if it doesn't do anything
	return true;
}

bool Window::onWindowFocusEvent(const WindowFocusEvent& e) {
	focused = e.focused;
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

	std::cout << "Window was resized" << std::endl;
	return true;
}


void Window::addResizeCallback(const std::function<void(int, int)>& callback) {
	resizeCallbacks.at(window).push_back(callback);
}

void Window::setup(const std::string& title, const int width, const int height) {
	if (GLFWWindowCount++ == 0) {
		int success = glfwInit();
		assert(success); // "GLFW could not be initialized!"
	}

	//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	bool success = window != nullptr;

	std::cout << std::format("Window created with title {} and dimensions {}x{}", title, width, height) << std::endl;

	glfwMakeContextCurrent(window);

	resizeCallbacks.emplace(window, std::vector<std::function<void(int, int)>>());
}

bool Window::onUpdate() {
	glfwSwapBuffers(window);
	return isRunning();
}
