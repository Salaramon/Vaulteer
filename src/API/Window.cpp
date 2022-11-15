#include "vpch.h"
#include "Window.h"

Window::Window(const std::string& title, unsigned const int width, unsigned const int height) : DebugLogger<Window>("WindowStuff") {
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
	return true;
}

bool Window::onWindowFocusEvent(const WindowFocusEvent& e) {
	//std::cout << "focused: " << e.focused << "\n";
	focused = e.focused;
	return true;
}

void Window::addResizeCallback(std::function<void(int, int)> callback) {
	resizeCallbacks.at(window).push_back(callback);
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
	return true;
}

/*void Window::onWindowIconifyEvent(const WindowIconifyEvent& e) {
	//if (iconified == GLFW_FALSE) {
	glfwMakeContextCurrent(window);
	//}
	//else {
	//	glfwMakeContextCurrent(nullptr);
	//}
}*/

void Window::setup(const std::string& title, const int width, const int height) {
	if (GLFWWindowCount++ == 0) {
		int success = glfwInit();
		assert(success, "GLFW could not be initialized!");
	}

	//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	bool success = window != nullptr;
	debug("GLFW window initialization: " + std::to_string(success) + "\n");
	
	glfwMakeContextCurrent(window);

	resizeCallbacks.emplace(window, std::vector<std::function<void(int, int)>>());
}

bool Window::onUpdate() {
	glfwSwapBuffers(window);
	return isRunning();
}
