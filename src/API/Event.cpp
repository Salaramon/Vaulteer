#include "vpch.h"
#include "Event.h"

std::future<void> Event::catchEvents;

Window* Event::window;

bool Event::startup = true;

double Event::lastPollTime = 0;
double Event::currentPollTime = 0;

void Event::addEventHandlingForWindow(Window* window) {
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

bool Event::poll() {
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


void Event::keyHandler(GLFWwindow* window, KeyCode key, ActionCode action) {
	StateCode enumState = static_cast<StateCode>(action);
	//Sets and stores the state of each button until changed.
	if (action == ActionCode::PRESS || action == ActionCode::RELEASE) {
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

double Event::now() {
	return currentPollTime;
}

double Event::delta() {
	return currentPollTime - lastPollTime;
}

bool Event::isKeyDown(KeyName& key) {
	return key >> Event::STATE::DOWN;
}

bool Event::isKeyUp(KeyName& key) {
	return key >> Event::STATE::UP;
}

bool Event::isKeyPressed(KeyName& key) {
	return key >> Event::ACTION::PRESS;
}

bool Event::isKeyReleased(KeyName& key) {
	return key >> Event::ACTION::RELEASE;
}

void Event::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	keyHandler(window, static_cast<KeyCode>(button + static_cast<int>(KeyCode::START_OF_MOUSEKEYS) + 1), static_cast<ActionCode>(action));
}

void Event::cursor_position_callback(GLFWwindow* window, double x, double y) {
	if (glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
		cursorEvents.push_back({
			.x = x,
			.y = y
		});
	}
}

void Event::scroll_callback(GLFWwindow* window, double x, double y) {
	if (glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
		wheelEvents.push_back({
			.ScrollX = x,
			.ScrollY = y
		});
	}
}

void Event::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	keyHandler(window, static_cast<KeyCode>(key), static_cast<ActionCode>(action));
}

/*
void Event::window_focus_callback(GLFWwindow* window, int key)
{

}
*/

Event::BooleanCheck operator<(const double lhs, const Event::CursorPosition& rhs) {
	return rhs.check(lhs, std::greater());
}

Event::BooleanCheck operator>(const double lhs, const Event::CursorPosition& rhs) {
	return rhs.check(lhs, std::less());
}

Event::BooleanCheck operator<=(const double lhs, const Event::CursorPosition& rhs) {
	return rhs.check(lhs, std::greater_equal());
}

Event::BooleanCheck operator>=(const double lhs, const Event::CursorPosition& rhs) {
	return rhs.check(lhs, std::less_equal());
}

Event::BooleanCheck operator<(const Event::CursorPosition& lhs, const double rhs) {
	return lhs.check(rhs, std::less());
}

Event::BooleanCheck operator>(const Event::CursorPosition& lhs, const double rhs) {
	return lhs.check(rhs, std::greater());
}

Event::BooleanCheck operator<=(const Event::CursorPosition& lhs, const double rhs) {
	return lhs.check(rhs, std::less_equal());
}

Event::BooleanCheck operator>=(const Event::CursorPosition& lhs, const double rhs) {
	return lhs.check(rhs, std::greater_equal());
}
