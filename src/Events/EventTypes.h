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
	MENU = 348,

	MOUSE_LEFT = 349,
	MOUSE_RIGHT = 350,
	MOUSE_MIDDLE = 351
};

enum class MouseKey {
	MOUSE_LEFT = 349,
	MOUSE_RIGHT = 350,
	MOUSE_MIDDLE = 351
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


#define FORWARD_THIS [this]<class EventType, auto fn>(EventType& e, Event::GenericWrapper<fn> fnWrap) -> bool { return (this->*fn)(e); }
#define FORWARD_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

// Base event data

enum EventType {
	None,
	WindowClose,
	WindowFullscreen,
	WindowMaximize,
	WindowResize,
	WindowFocus,
	KeyboardButtonAction,
	MouseButtonAction,
	MouseScroll,
	MouseMoved
};

#define EVENT_CLASS_TYPE(type) static EventType getStaticType() { return EventType::type; }\
							   virtual EventType getEventType() const override { return getStaticType(); }\
							   virtual const char* getName() const override { return #type; }

struct BaseEvent {
	bool handled = false;

	virtual EventType getEventType() const = 0;
	virtual const char* getName() const = 0;
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
	KeyboardButtonEvent(KeyboardButton button) : button(button) {}

	KeyboardButton button;

	EVENT_CLASS_TYPE(KeyboardButtonAction)
};

struct MouseButtonEvent : BaseEvent {
	MouseButtonEvent(MouseButton button) : button(button) {}

	MouseButton button;

	EVENT_CLASS_TYPE(MouseButtonAction)
};

struct MouseScrollEvent : BaseEvent {
	MouseScrollEvent(glm::vec2 mag) : mag(mag) {}

	glm::vec2 mag;

	EVENT_CLASS_TYPE(MouseScroll)
};

struct MouseMoveEvent : BaseEvent {
	MouseMoveEvent(Motion motion) : motion(motion) {}

	Motion motion;

	EVENT_CLASS_TYPE(MouseMoved)
};

struct WindowCloseEvent : BaseEvent {
	EVENT_CLASS_TYPE(WindowClose)
};

struct WindowFullscreenEvent : BaseEvent {
	WindowFullscreenEvent(bool fullscreen) : fullscreen(fullscreen) {}

	bool fullscreen;

	EVENT_CLASS_TYPE(WindowFullscreen)
};

struct WindowMaximizeEvent : BaseEvent {
	WindowMaximizeEvent(bool maximized) : maximized(maximized) {}

	bool maximized;

	EVENT_CLASS_TYPE(WindowMaximize)
};

struct WindowPositionEvent : BaseEvent {
	WindowPositionEvent(int xpos, int ypos) : xpos(xpos), ypos(ypos) {}

	int xpos, ypos;

	EVENT_CLASS_TYPE(WindowMaximize)
};

struct WindowFocusEvent : BaseEvent {
	WindowFocusEvent(bool focused) : focused(focused) {}

	bool focused;

	EVENT_CLASS_TYPE(WindowFocus)
};

struct WindowResizeEvent : BaseEvent {
	WindowResizeEvent(int width, int height) : width(width), height(height) {}

	int width, height;

	EVENT_CLASS_TYPE(WindowResize)
};
