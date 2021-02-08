#include "Event.h"


Event::_Key Event::Key;
Event::_Action Event::Action;
Event::_State Event::State;

Event::_CursorX Event::CursorX;
Event::_CursorY Event::CursorY;

Event::_Check Event::Check;
Event::_Count Event::Count;

std::vector<Event::Button> Event::buttonEvents;
std::vector<Event::Cursor> Event::cursorEvents;
std::vector<Event::Wheel> Event::wheelEvents;

std::future<void> Event::catchEvents;

Window* Event::window;

bool Event::startup = true;

double_t Event::lastPollTime = 0;
double_t Event::currentPollTime = 0;

Event::_Cursor Event::CursorEventList;

const std::array<intmax_t, Event::NUMBER_OF_POSSIBLE_KEYS> Event::allKeyValues{ -1, 32, 39, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 59, 61, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 96, 161, 162, 256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 280, 281, 282, 283, 284, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300, 301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 320, 321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335, 336, 340, 341, 342, 343, 344, 345, 346, 347, 348, 349, 350, 351 };


void Event::AddEventHandlingForWindow(Window* window)
{
	GLFWwindow* rawWindow = window->getRawWindow();
	Event::window = window;

	glfwSetMouseButtonCallback(rawWindow, mouse_button_callback);
	glfwSetCursorPosCallback(rawWindow, cursor_position_callback);
	glfwSetScrollCallback(rawWindow, scroll_callback);
	glfwSetKeyCallback(rawWindow, key_callback);
	//glfwSetCharCallback(rawWindow, char_callback);
	//glfwSetWindowFocusCallback(rawWindow, window_focus_callback);
	double x, y;
	glfwGetCursorPos(rawWindow, &x, &y);
	cursorEvents.push_back({
		.CursorX = x,
		.CursorY = y,
		.Time = static_cast<TIME>(now())
	});
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
	lastPollTime = currentPollTime;
	currentPollTime = glfwGetTime();
	
	//Carry over button down states
	std::vector<Button> remainingStateEvents;
	for (size_t i = 0; i < buttonEvents.size(); i++) {
		if (buttonEvents[i].State == STATE::DOWN) {
			buttonEvents[i].Time = static_cast<TIME>(-1);
			remainingStateEvents.push_back(buttonEvents[i]);
		}
	}
	
	buttonEvents.clear();
	Cursor currentLocation = cursorEvents.back();
	cursorEvents.clear();
	cursorEvents.push_back(currentLocation);
	wheelEvents.clear();
	
	glfwPollEvents();
	
	//Check for and remove if carried over down states is updated
	for (std::vector<Button>::iterator buttonCurrent = buttonEvents.begin(); buttonCurrent != buttonEvents.end(); buttonCurrent++) {
		for (std::vector<Button>::iterator buttonLeftover = remainingStateEvents.begin(); buttonLeftover != remainingStateEvents.end(); 0) {
			if (buttonCurrent->Key == buttonLeftover->Key && buttonCurrent->Action == ACTION::RELEASE) {
				buttonLeftover = remainingStateEvents.erase(buttonLeftover);
			}
			else {
				buttonLeftover++;
			}
		}
	}
	
	buttonEvents.insert(buttonEvents.begin(), remainingStateEvents.begin(), remainingStateEvents.end());
	
	//Add all the remaining keys that were not pressed
	std::vector<Button> existingEvents = buttonEvents;
	for (size_t i = 0; i < allKeyValues.size(); i++) {
		bool keyExists = false;
		for (size_t j = 0; j < existingEvents.size(); j++) {
			if (allKeyValues[i] == static_cast<int>(existingEvents[j].Key)) {
				keyExists = true;
				break;
			}
		}
		if (!keyExists) {
			buttonEvents.push_back({
				.Key = static_cast<KEY>(allKeyValues[i]),
				.Action = ACTION::NONE,
				.State = STATE::UP,
				.Time = static_cast<TIME>(-1)
			});
		}
	}
	
	return true;
}


double_t Event::now()
{
	return currentPollTime;
}

double_t Event::delta()
{
	return currentPollTime - lastPollTime;
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

Event::BooleanCheck Event::BooleanCheck::operator&&(BooleanCheck&& value)
{
	std::vector<size_t> eventsFound;
	for (size_t i = 0; i < eventIDs.size(); i++) {
		for(size_t j = 0; j < value.eventIDs.size(); j++)
		if (eventIDs[i] == value.eventIDs[j]) {
			eventsFound.push_back(eventIDs[i]);
		}
	}
	return BooleanCheck(eventsFound);
}

Event::BooleanCheck Event::_Key::operator==(KEY key)
{
	std::vector<size_t> eventsFound;
	for (size_t i = 0; i < buttonEvents.size(); i++) {
		if (buttonEvents[i].Key == key) {
			eventsFound.push_back(i);
		}
	}
	return BooleanCheck(eventsFound);
}

Event::BooleanCheck Event::_Action::operator==(ACTION action)
{
	std::vector<size_t> eventsFound;
	for (size_t i = 0; i < buttonEvents.size(); i++) {
		if (buttonEvents[i].Action == action) {
			eventsFound.push_back(i);
		}
	}
	return BooleanCheck(eventsFound);
}

Event::BooleanCheck Event::_State::operator==(STATE state)
{
	std::vector<size_t> eventsFound;
	for (size_t i = 0; i < buttonEvents.size(); i++) {
		if (buttonEvents[i].State == state) {
			eventsFound.push_back(i);
		}
	}

	return BooleanCheck(eventsFound);
}

void Event::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
		buttonEvents.push_back({
			.Key = static_cast<KEY>(button + static_cast<int>(KEY::START_OF_MOUSEKEYS) + 1),
			.Action = static_cast<ACTION>(action),
			.Time = static_cast<TIME>(now())
		});
	}
}

void Event::cursor_position_callback(GLFWwindow* window, double x, double y)
{
	if (glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
		cursorEvents.push_back({
			.CursorX = x,
			.CursorY = y,
			.Time = static_cast<TIME>(now())
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

	STATE state;
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		state = STATE::DOWN;
	}
	if (action == GLFW_RELEASE) {
		state = STATE::UP;
	}


	if (glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
		buttonEvents.push_back({
			.Key = static_cast<KEY>(key),
			.Action = static_cast<ACTION>(action),
			.State = state,
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

Event::BooleanCheck Event::_CursorX::operator<(CURSOR_X position)
{
	std::vector<size_t> eventsFound;
	for (size_t i = 0; i < cursorEvents.size(); i++) {
		if (cursorEvents[i].CursorX < position) {
			eventsFound.push_back(i);
		}
	}
	return BooleanCheck(eventsFound);
}

Event::BooleanCheck Event::_CursorX::operator>(CURSOR_X position)
{
	std::vector<size_t> eventsFound;
	for (size_t i = 0; i < cursorEvents.size(); i++) {
		if (cursorEvents[i].CursorX > position) {
			eventsFound.push_back(i);
		}
	}
	return BooleanCheck(eventsFound);
}

Event::BooleanCheck Event::_CursorY::operator<(CURSOR_Y position)
{
	std::vector<size_t> eventsFound;
	for (size_t i = 0; i < cursorEvents.size(); i++) {
		if (cursorEvents[i].CursorY < position) {
			eventsFound.push_back(i);
		}
	}
	return BooleanCheck(eventsFound);
}

Event::BooleanCheck Event::_CursorY::operator>(CURSOR_Y position)
{
	std::vector<size_t> eventsFound;
	for (size_t i = 0; i < cursorEvents.size(); i++) {
		if (cursorEvents[i].CursorY > position) {
			eventsFound.push_back(i);
		}
	}
	return BooleanCheck(eventsFound);
}

size_t Event::_Count::operator<<(BooleanCheck&& result)
{
	return result.eventIDs.size();
}

Event::CursorEvents Event::_Cursor::operator<<(BooleanCheck&& result)
{
	Event::CursorEvents events;
	for (size_t i = 0; i < result.eventIDs.size(); i++) {
		events.push_back(cursorEvents[result.eventIDs[i]]);
	}
	return events;
}
