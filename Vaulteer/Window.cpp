#include "Window.h"

Window::Window(const std::string title, unsigned const int width, unsigned const int height) 
{
	setup(title, width, height);
}

int Window::is_running()
{
	return !glfwWindowShouldClose(window);
}

GLFWwindow* Window::getRawWindow()
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
}

void Window::setup(const std::string title, unsigned const int width, unsigned const int height)
{
	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}
