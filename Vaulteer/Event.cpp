#include "vpch.h"
#include "Event.h"

std::future<void> Event::catchEvents;

Window* Event::window;

bool Event::startup = true;

double_t Event::lastPollTime = 0;
double_t Event::currentPollTime = 0;

void Event::AddEventHandlingForWindow(Window* window)
{
	DebugLogger<Event> log;
	GLFWwindow* rawWindow = window->getRawWindow();
	Event::window = window;

	log.debug("Event window context:" + std::to_string(window->getObjectKey()) + "\n");

	glfwSetMouseButtonCallback(rawWindow, mouse_button_callback);
	glfwSetCursorPosCallback(rawWindow, cursor_position_callback);
	glfwSetScrollCallback(rawWindow, scroll_callback);
	glfwSetKeyCallback(rawWindow, key_callback);
	//glfwSetWindowFocusCallback(rawWindow, window_focus_callback);

	double x, y;
	glfwGetCursorPos(rawWindow, &x, &y);
	cursorEvents.push_back({
		.x = x,
		.y = y
	});

}

bool Event::Poll() 
{
 	DebugLogger<Event> log;
	log.debug("Events started polling.\n");
	if (firstPoll) {
		currentPollTime = glfwGetTime();
		lastPollTime = currentPollTime;
		firstPoll = false;
	}
	else {
		lastPollTime = currentPollTime;
		currentPollTime = glfwGetTime();
	}
	Cursor carry = cursorEvents.back();
	buttonEvents.clear();
	cursorEvents.clear();
	cursorEvents.push_back(carry);
	wheelEvents.clear();
	
	glfwPollEvents();

	log.debug("Events finished polling.\n");

	return true;
}


void Event::keyHandler(GLFWwindow* window, enumKEY key, enumACTION action)
{
	enumSTATE enumState = static_cast<enumSTATE>(action);
	//Sets and stores the state of each button until changed.
	if (action == enumACTION::PRESS || action == enumACTION::RELEASE) {
		auto it = buttonStates.find(key);
		if (it != buttonStates.end()) {
			it->second = enumState;
		}
		else {
			buttonStates.emplace(key, enumState);
		}
	}

	//Pushes back all keyboard key events that happened.
	if (glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
		buttonEvents.push_back({
			.key = key,
			.action = action
		});
	}
}

double_t Event::now()
{
	return currentPollTime;
}

double_t Event::delta()
{
	return currentPollTime - lastPollTime;
}

void Event::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	keyHandler(window, static_cast<enumKEY>(button + static_cast<int>(enumKEY::START_OF_MOUSEKEYS) + 1), static_cast<enumACTION>(action));
}

void Event::cursor_position_callback(GLFWwindow* window, double x, double y)
{
	if (glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
		cursorEvents.push_back({
			.x = x,
			.y = y
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
	keyHandler(window, static_cast<enumKEY>(key), static_cast<enumACTION>(action));
}

/*
void Event::window_focus_callback(GLFWwindow* window, int key)
{

}
*/

Event::BooleanCheck operator<(const double_t lhs, const Event::CursorPosition& rhs)
{
	return rhs.check(lhs, std::greater<double_t>());
}
 Event::BooleanCheck operator>(const double_t lhs, const Event::CursorPosition& rhs)
{
	return rhs.check(lhs, std::less<double_t>());
}
 Event::BooleanCheck operator<=(const double_t lhs, const Event::CursorPosition& rhs)
{
	return rhs.check(lhs, std::greater_equal<double_t>());
}
 Event::BooleanCheck operator>=(const double_t lhs, const Event::CursorPosition& rhs)
{
	return rhs.check(lhs, std::less_equal<double_t>());
}

 Event::BooleanCheck operator<(const Event::CursorPosition& lhs, const double_t rhs)
{
	return lhs.check(rhs, std::less<double_t>());
}
 Event::BooleanCheck operator>(const Event::CursorPosition& lhs, const double_t rhs)
{
	return lhs.check(rhs, std::greater<double_t>());
}
 Event::BooleanCheck operator<=(const Event::CursorPosition& lhs, const double_t rhs)
{
	return lhs.check(rhs, std::less_equal<double_t>());
}
 Event::BooleanCheck operator>=(const Event::CursorPosition& lhs, const double_t rhs)
{
	return lhs.check(rhs, std::greater_equal<double_t>());
}
