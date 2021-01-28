#include "Event.h"


typedef double_t TIME;
typedef double_t CURSOR_X;
typedef double_t CURSOR_Y;
typedef double_t SCROLL_X;
typedef double_t SCROLL_Y;
typedef Window WINDOW_CONTEXT;

void Event::AddEventHandlingForWindow(Window* window)
{
	GLFWwindow* rawWindow = window->getRawWindow();
	windowContexts.emplace(window);

	glfwSetMouseButtonCallback(rawWindow, mouse_button_callback);
	glfwSetCursorPosCallback(rawWindow, cursor_position_callback);
	glfwSetScrollCallback(rawWindow, scroll_callback);
	glfwSetKeyCallback(rawWindow, key_callback);
	//glfwSetCharCallback(rawWindow, char_callback);
	//glfwSetWindowFocusCallback(rawWindow, window_focus_callback);
}

TIME Event::getTime(EventID id)
{
	if (id != 0) {
		if (id <= buttonEvents.size() + 1) {
			return buttonEvents[id-1].Time;
		}
		else if (id <= cursorEvents.size() + buttonEvents.size() + 1) {
			return cursorEvents[id - buttonEvents.size() - 1].Time;
		}
		else if (id <= wheelEvents.size() + cursorEvents.size() + buttonEvents.size() + 1) {
			return wheelEvents[id - cursorEvents.size() - buttonEvents.size() - 1].Time;
		}
	}
	return 0;
}

bool Event::Poll() 
{
	glfwPostEmptyEvent();
	catchEvents.wait();
	for (auto window : windowContexts) {
		if (glfwWindowShouldClose(window->getRawWindow())) {
			windowContexts.erase(window);
			if (windowContexts.empty()) {
				return false;
			}
		}
	}
	catchEvents = std::async(glfwWaitEvents);
	return true;
}


double_t Event::now()
{
	return static_cast<double_t>(glfwGetTime());
}

size_t Event::Key::operator()(KEY key)
{
	for (size_t i = 0; i < buttonEvents.size(); i++) {
		if (buttonEvents[i].Key == key) {
			return i+1;
		}
	}
	return 0;
}

size_t Event::Key::operator()(KEY key, ACTION action)
{
	for (size_t i = 0; i < buttonEvents.size(); i++) {
		if (buttonEvents[i].Key == key && buttonEvents[i].Action == action) {
			return i + 1;
		}
	}
	return 0;
}

size_t Event::Key::operator()(ACTION action)
{
	for (size_t i = 0; i < buttonEvents.size(); i++) {
		if (buttonEvents[i].Action == action) {
			return i + 1;
		}
	}
	return 0;
}

size_t Event::Key::operator()(KEY key, STATE state)
{
	for (size_t i = 0; i < buttonEvents.size(); i++) {
		if (buttonEvents[i].Key == key && buttonEvents[i].State == state) {
			return i + 1;
		}
	}
	return 0;
}

void Event::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
		buttonEvents.push_back({
			.Key = static_cast<KEY>(button + static_cast<int>(KEY::START_OF_MOUSEKEYS) + 1),
			.Action = static_cast<ACTION>(action),
			.State = static_cast<STATE>(glfwGetMouseButton(window,button)),
			.Time = static_cast<TIME>(now())
		});
	}
}

void Event::cursor_position_callback(GLFWwindow* window, double x, double y)
{
	if (glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
		cursorEvents.push_back({
			.CursorX = x,
			.CursorY = y
		});
	}
}

void Event::scroll_callback(GLFWwindow* window, double x, double y)
{
	if (glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
		wheelEvents.push_back({
			.ScrollX = x,
			.ScrollY = y
		});
	}
}

void Event::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	if (glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
		buttonEvents.push_back({
			.Key = static_cast<KEY>(scancode),
			.Action = static_cast<ACTION>(action),
			.State = static_cast<STATE>(glfwGetKey(window,key)),
			.Time = static_cast<TIME>(now())
		});
	}
}

/*
void Event::char_callback(GLFWwindow* window, unsigned int codepoint)
{

}
*/
/*
void Event::window_focus_callback(GLFWwindow* window, int key)
{

}
*/