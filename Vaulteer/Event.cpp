#include "Event.h"


Event::_Key Event::Key;
Event::_Action Event::Action;
Event::_State Event::State;
Event::_Check Event::Check;


std::vector<Event::Button> Event::buttonEvents;
std::vector<Event::Cursor> Event::cursorEvents;
std::vector<Event::Wheel> Event::wheelEvents;

std::future<void> Event::catchEvents;

std::unordered_set<Window*> Event::windowContexts;

bool Event::startup = true;


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

Event::TIME Event::getTime(EventID id)
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
	
	
	return true;
}


double_t Event::now()
{
	return static_cast<double_t>(glfwGetTime());
}

bool Event::_Check::operator<<(BooleanCheck&& result)
{
	return result.eventHappened();
}

Event::BooleanCheck::BooleanCheck(std::vector<size_t> value)
{
	eventIDs = value;
}

bool Event::BooleanCheck::eventHappened()
{
	return !eventIDs.empty();
}

Event::BooleanCheck&& Event::BooleanCheck::operator&&(BooleanCheck&& value)
{
	std::vector<size_t> eventsFound;
	for (size_t i = 0; i < eventIDs.size(); i++) {
		for(size_t j = 0; j < value.eventIDs.size(); j++)
		if (eventIDs[i] == value.eventIDs[j]) {
			eventsFound.push_back(eventIDs[i]);
		}
	}
	return eventsFound;
}

Event::BooleanCheck&& Event::_Key::operator==(KEY key)
{
	std::vector<size_t> eventsFound;
	for (size_t i = 0; i < buttonEvents.size(); i++) {
		if (buttonEvents[i].Key == key) {
			eventsFound.push_back(i);
		}
	}
	return eventsFound;
}

Event::BooleanCheck&& Event::_Action::operator==(ACTION action)
{
	std::vector<size_t> eventsFound;
	for (size_t i = 0; i < buttonEvents.size(); i++) {
		if (buttonEvents[i].Action == action) {
			eventsFound.push_back(i);
		}
	}
	return eventsFound;
}

Event::BooleanCheck&& Event::_State::operator==(STATE state)
{
	std::vector<size_t> eventsFound;
	for (size_t i = 0; i < buttonEvents.size(); i++) {
		if (buttonEvents[i].State == state) {
			eventsFound.push_back(i);
		}
	}
	return eventsFound;
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
