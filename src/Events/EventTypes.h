#pragma once

#include <glfw3.h>


enum class KeyboardKey {
	UNKNOWN = -1,
	SPACE = 32,
	APOSTROPHE = 39,
	COMMA = 44,
	MINUS = 45,
	PERIOD = 46,
	SLASH = 47,
	_0 = 48,
	_1 = 49,
	_2 = 50,
	_3 = 51,
	_4 = 52,
	_5 = 53,
	_6 = 54,
	_7 = 55,
	_8 = 56,
	_9 = 57,
	SEMICOLON = 59,
	EQUAL = 61,
	A = 65,
	B = 66,
	C = 67,
	D = 68,
	E = 69,
	F = 70,
	G = 71,
	H = 72,
	I = 73,
	J = 74,
	K = 75,
	L = 76,
	M = 77,
	N = 78,
	O = 79,
	P = 80,
	Q = 81,
	R = 82,
	S = 83,
	T = 84,
	U = 85,
	V = 86,
	W = 87,
	X = 88,
	Y = 89,
	Z = 90,
	LEFT_BRACKET = 91,
	BACKSLASH = 92,
	RIGHT_BRACKET = 93,
	GRAVE_ACCENT = 96,
	WORLD_1 = 161,
	WORLD_2 = 162,
	ESCAPE = 256,
	ENTER = 257,
	TAB = 258,
	BACKSPACE = 259,
	INSERT = 260,
	DELETE = 261,
	RIGHT = 262,
	LEFT = 263,
	DOWN = 264,
	UP = 265,
	PAGE_UP = 266,
	PAGE_DOWN = 267,
	HOME = 268,
	END = 269,
	CAPS_LOCK = 280,
	SCROLL_LOCK = 281,
	NUM_LOCK = 282,
	PRINT_SCREEN = 283,
	PAUSE = 284,
	F1 = 290,
	F2 = 291,
	F3 = 292,
	F4 = 293,
	F5 = 294,
	F6 = 295,
	F7 = 296,
	F8 = 297,
	F9 = 298,
	F10 = 299,
	F11 = 300,
	F12 = 301,
	F13 = 302,
	F14 = 303,
	F15 = 304,
	F16 = 305,
	F17 = 306,
	F18 = 307,
	F19 = 308,
	F20 = 309,
	F21 = 310,
	F22 = 311,
	F23 = 312,
	F24 = 313,
	F25 = 314,
	KP_0 = 320,
	KP_1 = 321,
	KP_2 = 322,
	KP_3 = 323,
	KP_4 = 324,
	KP_5 = 325,
	KP_6 = 326,
	KP_7 = 327,
	KP_8 = 328,
	KP_9 = 329,
	KP_DECIMAL = 330,
	KP_DIVIDE = 331,
	KP_MULTIPLY = 332,
	KP_SUBTRACT = 333,
	KP_ADD = 334,
	KP_ENTER = 335,
	KP_EQUAL = 336,
	LEFT_SHIFT = 340,
	LEFT_CONTROL = 341,
	LEFT_ALT = 342,
	LEFT_SUPER = 343,
	RIGHT_SHIFT = 344,
	RIGHT_CONTROL = 345,
	RIGHT_ALT = 346,
	RIGHT_SUPER = 347,
	MENU = 348
};

enum class MouseKey {
	MOUSE_LEFT = GLFW_MOUSE_BUTTON_LEFT,
	MOUSE_RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
	MOUSE_MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE
};

enum class KeyAction {
	RELEASE,
	PRESS,
	REPEAT,
};

enum class KeyState {
	UP,
	DOWN
};

inline bool operator||(KeyState l, KeyState r) {
	return (int)l | (int)r;
}


#define FORWARD_THIS(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
#define FORWARD_FN(fn) [](auto&&... args) -> decltype(auto) { return fn(std::forward<decltype(args)>(args)...); }

// Base event data

enum EventType {
	None,
	WindowClose,
	WindowFullscreen,
	WindowMaximize,
	WindowPosition,
	WindowResize,
	WindowFocus,
	KeyboardButtonAction,
	MouseButtonAction,
	MouseScroll,
	MouseMoved
};

#define EVENT_CLASS_TYPE(type) static EventType getStaticType() { return EventType::type; }

struct BaseEvent {
	EventType type = None;
	bool handled = false;
};

// Event utils

struct KeyboardButton {
	KeyboardKey key;
	KeyAction action;
};

struct MouseButton {
	MouseKey key;
	KeyAction action;
};

struct Motion {
	glm::vec2 position;
	glm::vec2 delta;
};

// Event implementations

struct KeyboardButtonEvent : BaseEvent {
	KeyboardButton button;

	KeyboardButtonEvent(KeyboardButton input) : BaseEvent{KeyboardButtonAction}, button(input) {}

	EVENT_CLASS_TYPE(KeyboardButtonAction)

	bool checkPress(KeyboardKey key) const {
		return button.key == key && button.action == KeyAction::PRESS;
	}
	bool checkRelease(KeyboardKey key) const {
		return button.key == key && button.action == KeyAction::RELEASE;
	}
	bool checkRepeat(KeyboardKey key) const {
		return button.key == key && button.action == KeyAction::REPEAT;
	}
};

struct MouseButtonEvent : BaseEvent {
	MouseButton button;

	MouseButtonEvent(MouseButton input) : BaseEvent{MouseButtonAction}, button(input) {}

	bool checkPress(MouseKey key) const {
		return button.key == key && button.action == KeyAction::PRESS;
	}
	bool checkRelease(MouseKey key) const {
		return button.key == key && button.action == KeyAction::RELEASE;
	}

	EVENT_CLASS_TYPE(MouseButtonAction)
};

struct MouseScrollEvent : BaseEvent {
	glm::vec2 mag;

	MouseScrollEvent(glm::vec2 input) : BaseEvent{MouseScroll}, mag(input) {}

	EVENT_CLASS_TYPE(MouseScroll)
};

struct MouseMoveEvent : BaseEvent {
	Motion motion; 

	MouseMoveEvent(Motion input) : BaseEvent{MouseScroll}, motion(input) {}

	EVENT_CLASS_TYPE(MouseScroll)
};

struct WindowCloseEvent : BaseEvent {
	WindowCloseEvent() : BaseEvent{WindowClose} {}

	EVENT_CLASS_TYPE(WindowClose)
};

struct WindowFullscreenEvent : BaseEvent {
	bool fullscreen;

	WindowFullscreenEvent(bool fullscreen) : BaseEvent{WindowFullscreen}, fullscreen(fullscreen) {}

	EVENT_CLASS_TYPE(WindowFullscreen)
};

struct WindowMaximizeEvent : BaseEvent {
	bool maximised;

	WindowMaximizeEvent(bool maximised) : BaseEvent{WindowMaximize}, maximised(maximised) {}

	EVENT_CLASS_TYPE(WindowMaximize)
};

struct WindowPositionEvent : BaseEvent {
	int xpos, ypos;

	WindowPositionEvent(int xpos, int ypos) : BaseEvent{WindowPosition}, xpos(xpos), ypos(ypos) {}

	EVENT_CLASS_TYPE(WindowPosition)
};

struct WindowFocusEvent : BaseEvent {
	bool focused;

	WindowFocusEvent(bool focused) : BaseEvent{WindowFocus}, focused(focused) {}

	EVENT_CLASS_TYPE(WindowFocus)
};

struct WindowResizeEvent : BaseEvent {
	int width, height;

	WindowResizeEvent(int width, int height) : BaseEvent{WindowResize}, width(width), height(height) {}

	EVENT_CLASS_TYPE(WindowResize)
};
