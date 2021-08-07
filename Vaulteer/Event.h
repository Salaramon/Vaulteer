#pragma once

#include <vector>
#include <future>
#include <algorithm>
#include <set>
#include <chrono>
#include <array>

#include "Window.h"
#include "DebugLogger.h"

class Event : public DebugLogger<Event>
{
public:

	typedef double_t TIME;
	typedef double_t CURSOR_X;
	typedef double_t CURSOR_Y;
	typedef double_t SCROLL_X;
	typedef double_t SCROLL_Y;
	typedef Window WINDOW_CONTEXT;

	static void AddEventHandlingForWindow(Window* window);

	static bool Poll();

	typedef size_t EventID;

	TIME getTime(EventID id);

	enum class KEY {
		UNKNOWN = -1,
		START_OF_KEYBOARDKEYS = KEY::UNKNOWN,
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

		START_OF_MOUSEKEYS = KEY::MENU,
		MOUSE_LEFT = 349,
		MOUSE_RIGHT = 350,
		MOUSE_MIDDLE = 351
	};

	enum class ACTION {
		RELEASE = 0,
		PRESS = 1,
		REPEAT = 2,
		NONE = 3
	};

	enum class STATE {
		UP = 0,
		DOWN = 1
	};

	struct BooleanCheck {
		BooleanCheck(std::vector<size_t> value);
		bool eventHappened();
		std::vector<size_t> eventIDs;
		BooleanCheck operator&&(BooleanCheck&& value);
	};

	struct _Check {
		bool operator<<(BooleanCheck&& result);
	};
	
	struct _Count {
		size_t operator<<(BooleanCheck&& result);
	};
	


	struct _Key {
		BooleanCheck operator==(KEY key);
	};
	struct _Action {
		BooleanCheck operator==(ACTION key);
	};
	struct _State {
		BooleanCheck operator==(STATE state);
	};

	struct Time {
		template<class FLOAT_TYPE>
		bool operator<(FLOAT_TYPE timePoint);
		template<class FLOAT_TYPE>
		bool operator>(FLOAT_TYPE timePoint);
		template<class FLOAT_TYPE>
		bool operator<=(FLOAT_TYPE timePoint);
		template<class FLOAT_TYPE>
		bool operator>=(FLOAT_TYPE timePoint);
		double_t timeStamp;
	};
	struct _CursorX{
		BooleanCheck operator<(CURSOR_X position);
		BooleanCheck operator>(CURSOR_X position);
		BooleanCheck operator<=(CURSOR_X position);
		BooleanCheck operator>=(CURSOR_X position);
	};
	struct _CursorY {
		BooleanCheck operator<(CURSOR_Y position);
		BooleanCheck operator>(CURSOR_Y position);
		BooleanCheck operator<=(CURSOR_Y position);
		BooleanCheck operator>=(CURSOR_Y position);
	};
	struct ScrollX {
		template<class FLOAT_TYPE>
		bool operator<(FLOAT_TYPE timePoint);
		template<class FLOAT_TYPE>
		bool operator>(FLOAT_TYPE timePoint);
		template<class FLOAT_TYPE>
		bool operator<=(FLOAT_TYPE timePoint);
		template<class FLOAT_TYPE>
		bool operator>=(FLOAT_TYPE timePoint);
		SCROLL_X pos;
	};
	struct ScrollY {
		template<class FLOAT_TYPE>
		bool operator<(FLOAT_TYPE timePoint);
		template<class FLOAT_TYPE>
		bool operator>(FLOAT_TYPE timePoint);
		template<class FLOAT_TYPE>
		bool operator<=(FLOAT_TYPE timePoint);
		template<class FLOAT_TYPE>
		bool operator>=(FLOAT_TYPE timePoint);
		SCROLL_Y pos;
	};
	struct WindowContext {
		bool operator==(WINDOW_CONTEXT windowContext);
	};

	struct Button {
		KEY Key;
		ACTION Action;
		STATE State;
		TIME Time;
	};
	struct Cursor {
		CURSOR_X CursorX;
		CURSOR_Y CursorY;
		TIME Time;
	};
	struct Wheel {
		SCROLL_X ScrollX;
		SCROLL_Y ScrollY;
		TIME Time;
	};

	typedef std::vector<Button> KeyEvents;
	typedef std::vector<Cursor> CursorEvents;


	struct _Cursor {
		CursorEvents operator<<(BooleanCheck&& result);
	};

	static _Key Key;
	static _Action Action;
	static _State State;

	static _CursorX CursorX;
	static _CursorY CursorY;

	static _Check Check;
	static _Count Count;

	static _Cursor CursorEventList;

	static double_t now();
	static double_t delta();

private:

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double x, double y);
	static void scroll_callback(GLFWwindow* window, double x, double y);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	//static void char_callback(GLFWwindow* window, unsigned int codepoint);
	//static void window_focus_callback(GLFWwindow* window, int focused);

	static std::vector<Button> buttonEvents;
	static std::vector<Cursor> cursorEvents;
	static std::vector<Wheel> wheelEvents;
	
	static std::future<void> catchEvents;
	//static std::unordered_map<Event*,Window*> windowContexts;
	static Window* window;

	static bool startup;

	static double_t lastPollTime;
	static double_t currentPollTime;

	static const size_t NUMBER_OF_POSSIBLE_KEYS = 124;
	static const std::array<intmax_t, NUMBER_OF_POSSIBLE_KEYS> allKeyValues;
};

//Create own value type to make compact position checking

/*
	Have the ID as return value, but allow that ID or Type to be used in conjunction with Time and other things such as Event::Key(Event::KEY::A) &&
	Event::Key(Event::KEY::A) returning type with && operator (May not work due to not being able to return a value if no such operator is done.
	Maybe Event::Check << (Conditions here)
	Where Check << basically just runs a function to get true or false through using a interfaced function
*/