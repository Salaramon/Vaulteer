#include "vpch.h"
#include "Window.h"

#include "Events/Event.h"

Window::Window(WindowSpecification spec) {
	Window::specification = spec;
	setup();

	int init = !gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
}

Window::~Window() {
	
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

int Window::isRunning() {
	return !glfwWindowShouldClose(window);
}

bool Window::onWindowCloseEvent(const WindowCloseEvent& e) {
	// registering the closing status - but actual closing is handled by GLFW
	isClosing = true;
	return true;
}

bool Window::onWindowFullscreenEvent(const WindowFullscreenEvent& e) {
	fullscreen = e.fullscreen;
	if (fullscreen) {
		auto* monitor = glfwGetPrimaryMonitor();
		auto* mode = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	} else {
		auto* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		glfwSetWindowMonitor(window, nullptr, specification.x, specification.y, specification.width, specification.height, mode->refreshRate);
	}
	return true;
}

bool Window::onWindowMaximizeEvent(const WindowMaximizeEvent& e) {
	maximized = e.maximized;
	return true; 
}

bool Window::onWindowPositionEvent(const WindowPositionEvent& e) {
	// stops the window from travelling offscreen in special circumstances
	if (!fullscreen) {
		specification.x = std::max(e.xpos, 0);
		specification.y = std::max(e.ypos, 0);
	}
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
	
	if (!fullscreen) {
		specification.width = e.width;
		specification.height = e.height;
	}

	std::cout << std::format("Window was resized to {}x{}", e.width, e.height) << std::endl;
	return true;
}


void Window::addResizeCallback(const std::function<void(int, int)>& callback) {
	resizeCallbacks.at(window).push_back(callback);
}

void Window::setup() {
	//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(specification.width, specification.height, specification.title.c_str(), nullptr, nullptr);
	bool success = window != nullptr;
	assert(success);  // "Window could not be initialized!"

	glfwSetWindowPos(window, specification.x, specification.y);
	glfwMakeContextCurrent(window);

	bool vsync = false; 
	glfwSwapInterval(vsync ? 1 : 0);

	resizeCallbacks.emplace(window, std::vector<std::function<void(int, int)>>());

	std::cout << std::format("Window created with title {} and dimensions {}x{}", specification.title, specification.width, specification.height) << std::endl;
}

bool Window::onUpdate() {
	glfwSwapBuffers(window);
	return isRunning();
}
