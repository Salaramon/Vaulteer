#pragma once

#include <vector>
#include <future>
#include <algorithm>
#include <set>
#include <chrono>
#include <array>
#include <unordered_map>
#include <functional>

#include "Window.h"
#include "DebugLogger.h"

class Event : public DebugLogger<Event>
{
public:

	static void AddEventHandlingForWindow(Window* window);

	static bool Poll();


	enum class KeyCode {
		UNKNOWN = -1,
		START_OF_KEYBOARDKEYS = KeyCode::UNKNOWN,
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

		START_OF_MOUSEKEYS = KeyCode::MENU,
		MOUSE_LEFT = 349,
		MOUSE_RIGHT = 350,
		MOUSE_MIDDLE = 351
	};

	enum class ActionCode {
		RELEASE = 0,
		PRESS = 1,
		REPEAT = 2,
	};

	enum class StateCode {
		UP = 0,
		DOWN = 1
	};

	struct Button {
		KeyCode key;
		ActionCode action;
	};
	struct Cursor {
		double x;
		double y;
	};
	struct Wheel {
		double ScrollX;
		double ScrollY;
	};


	struct BooleanCheck {
		BooleanCheck(bool result) : result(result) {}
		BooleanCheck operator&&(BooleanCheck check){
			return check.result && result;
		}
		BooleanCheck operator||(BooleanCheck check) {
			return check.result || result;
		}
		operator bool() {
			return result;
		}

	private:
		bool result;
	};

	struct KeyAction {
	public:
		KeyAction(ActionCode actionID) : actionID(actionID) {}
		
		ActionCode actionID;

		std::vector<Button> possibleEvents;
	};

	struct KeyState {
	public:
		KeyState(StateCode stateID) : stateID(stateID) {}

		StateCode stateID;
	};

	struct KeyName {
	public:
		KeyName(KeyCode keyID) : keyID(keyID){}

		BooleanCheck operator>>(KeyAction& data) {
			for (auto& button : buttonEvents) {
				if (button.key == keyID && button.action == data.actionID) {
					return true;
				}
			}
			return false;
		}
		BooleanCheck operator>>(KeyState& data) {
			auto it = buttonStates.find(keyID);
			if(it != buttonStates.end()) {
				return it->second == data.stateID;
			}
			return false;
		}

		KeyCode keyID;
	};

	struct CursorPosition {
	public:
		CursorPosition(size_t offset) : offset(offset) {}
		//bool operator<(doubleConverter axis) { return check(axis, std::less<bool>()); }
		//bool operator>(doubleConverter axis) { return check(axis, std::greater<bool>()); }
		//bool operator<=(doubleConverter axis) { return check(axis, std::less_equal<bool>()); }
		//bool operator>=(doubleConverter axis) { return check(axis, std::greater_equal<bool>()); }

		friend BooleanCheck operator<(const double lhs, const CursorPosition& rhs);
		friend BooleanCheck operator>(const double lhs, const CursorPosition& rhs);
		friend BooleanCheck operator<=(const double lhs, const CursorPosition& rhs);
		friend BooleanCheck operator>=(const double lhs, const CursorPosition& rhs);
		friend BooleanCheck operator<(const CursorPosition& lhs, const double rhs);
		friend BooleanCheck operator>(const CursorPosition& lhs, const double rhs);
		friend BooleanCheck operator<=(const CursorPosition& lhs, const double rhs);
		friend BooleanCheck operator>=(const CursorPosition& lhs, const double rhs);

		template<class Func>
		bool check(double axis, Func func) const {
			for (auto& pos : cursorEvents) {
				double cur = *(reinterpret_cast<double*>((&pos)) + (offset / sizeof(double)));
				if (func(cur, axis)) {
					return true;
				}
			}
			return false;
		}

		double get() const {
			if (!cursorEvents.empty()) {
				double cur = *(reinterpret_cast<double*>((&cursorEvents.back())) + (offset / sizeof(double)));
				return cur;
			}
			else {
				return 0;
			}
		}

		double delta() const {
			if (!cursorEvents.empty()) {
				double front = *(reinterpret_cast<double*>((&cursorEvents.front())) + (offset / sizeof(double)));
				double back = *(reinterpret_cast<double*>((&cursorEvents.back())) + (offset / sizeof(double)));
				return back - front;
			}
			else {
				return 0;
			}
		}

		const size_t offset;
	};

	struct KEY {
		inline static KeyName UNKNOWN{ KeyCode::UNKNOWN };
		inline static KeyName SPACE{ KeyCode::SPACE };
		inline static KeyName APOSTROPHE{ KeyCode::APOSTROPHE };
		inline static KeyName COMMA{ KeyCode::COMMA };
		inline static KeyName MINUS{ KeyCode::MINUS };
		inline static KeyName PERIOD{ KeyCode::PERIOD };
		inline static KeyName SLASH{ KeyCode::SLASH };
		inline static KeyName _0{ KeyCode::_0 };
		inline static KeyName _1{ KeyCode::_1 };
		inline static KeyName _2{ KeyCode::_2 };
		inline static KeyName _3{ KeyCode::_3 };
		inline static KeyName _4{ KeyCode::_4 };
		inline static KeyName _5{ KeyCode::_5 };
		inline static KeyName _6{ KeyCode::_6 };
		inline static KeyName _7{ KeyCode::_7 };
		inline static KeyName _8{ KeyCode::_8 };
		inline static KeyName _9{ KeyCode::_9 };
		inline static KeyName SEMICOLON{ KeyCode::SEMICOLON };
		inline static KeyName EQUAL{ KeyCode::EQUAL };
		inline static KeyName A{ KeyCode::A };
		inline static KeyName B{ KeyCode::B };
		inline static KeyName C{ KeyCode::C };
		inline static KeyName D{ KeyCode::D };
		inline static KeyName E{ KeyCode::E };
		inline static KeyName F{ KeyCode::F };
		inline static KeyName G{ KeyCode::G };
		inline static KeyName H{ KeyCode::H };
		inline static KeyName I{ KeyCode::I };
		inline static KeyName J{ KeyCode::J };
		inline static KeyName K{ KeyCode::K };
		inline static KeyName L{ KeyCode::L };
		inline static KeyName M{ KeyCode::M };
		inline static KeyName N{ KeyCode::N };
		inline static KeyName O{ KeyCode::O };
		inline static KeyName P{ KeyCode::P };
		inline static KeyName Q{ KeyCode::Q };
		inline static KeyName R{ KeyCode::R };
		inline static KeyName S{ KeyCode::S };
		inline static KeyName T{ KeyCode::T };
		inline static KeyName U{ KeyCode::U };
		inline static KeyName V{ KeyCode::V };
		inline static KeyName W{ KeyCode::W };
		inline static KeyName X{ KeyCode::X };
		inline static KeyName Y{ KeyCode::Y };
		inline static KeyName Z{ KeyCode::Z };
		inline static KeyName LEFT_BRACKET{ KeyCode::LEFT_BRACKET };
		inline static KeyName BACKSLASH{ KeyCode::BACKSLASH };
		inline static KeyName RIGHT_BRACKET{ KeyCode::RIGHT_BRACKET };
		inline static KeyName GRAVE_ACCENT{ KeyCode::GRAVE_ACCENT };
		inline static KeyName WORLD_1{ KeyCode::WORLD_1 };
		inline static KeyName WORLD_2{ KeyCode::WORLD_2 };
		inline static KeyName ESCAPE{ KeyCode::ESCAPE };
		inline static KeyName ENTER{ KeyCode::ENTER };
		inline static KeyName TAB{ KeyCode::TAB };
		inline static KeyName BACKSPACE{ KeyCode::BACKSPACE };
		inline static KeyName INSERT{ KeyCode::INSERT };
		inline static KeyName DELETE{ KeyCode::DELETE };
		inline static KeyName RIGHT{ KeyCode::RIGHT };
		inline static KeyName LEFT{ KeyCode::LEFT };
		inline static KeyName DOWN{ KeyCode::DOWN };
		inline static KeyName UP{ KeyCode::UP };
		inline static KeyName PAGE_UP{ KeyCode::PAGE_UP };
		inline static KeyName PAGE_DOWN{ KeyCode::PAGE_DOWN };
		inline static KeyName HOME{ KeyCode::HOME };
		inline static KeyName END{ KeyCode::END };
		inline static KeyName CAPS_LOCK{ KeyCode::CAPS_LOCK };
		inline static KeyName SCROLL_LOCK{ KeyCode::SCROLL_LOCK };
		inline static KeyName NUM_LOCK{ KeyCode::NUM_LOCK };
		inline static KeyName PRINT_SCREEN{ KeyCode::PRINT_SCREEN };
		inline static KeyName PAUSE{ KeyCode::PAUSE };
		inline static KeyName F1{ KeyCode::F1 };
		inline static KeyName F2{ KeyCode::F2 };
		inline static KeyName F3{ KeyCode::F3 };
		inline static KeyName F4{ KeyCode::F4 };
		inline static KeyName F5{ KeyCode::F5 };
		inline static KeyName F6{ KeyCode::F6 };
		inline static KeyName F7{ KeyCode::F7 };
		inline static KeyName F8{ KeyCode::F8 };
		inline static KeyName F9{ KeyCode::F9 };
		inline static KeyName F10{ KeyCode::F10 };
		inline static KeyName F11{ KeyCode::F11 };
		inline static KeyName F12{ KeyCode::F12 };
		inline static KeyName F13{ KeyCode::F13 };
		inline static KeyName F14{ KeyCode::F14 };
		inline static KeyName F15{ KeyCode::F15 };
		inline static KeyName F16{ KeyCode::F16 };
		inline static KeyName F17{ KeyCode::F17 };
		inline static KeyName F18{ KeyCode::F18 };
		inline static KeyName F19{ KeyCode::F19 };
		inline static KeyName F20{ KeyCode::F20 };
		inline static KeyName F21{ KeyCode::F21 };
		inline static KeyName F22{ KeyCode::F22 };
		inline static KeyName F23{ KeyCode::F23 };
		inline static KeyName F24{ KeyCode::F24 };
		inline static KeyName F25{ KeyCode::F25 };
		inline static KeyName KP_0{ KeyCode::KP_0 };
		inline static KeyName KP_1{ KeyCode::KP_1 };
		inline static KeyName KP_2{ KeyCode::KP_2 };
		inline static KeyName KP_3{ KeyCode::KP_3 };
		inline static KeyName KP_4{ KeyCode::KP_4 };
		inline static KeyName KP_5{ KeyCode::KP_5 };
		inline static KeyName KP_6{ KeyCode::KP_6 };
		inline static KeyName KP_7{ KeyCode::KP_7 };
		inline static KeyName KP_8{ KeyCode::KP_8 };
		inline static KeyName KP_9{ KeyCode::KP_9 };
		inline static KeyName KP_DECIMAL{ KeyCode::KP_DECIMAL };
		inline static KeyName KP_DIVIDE{ KeyCode::KP_DIVIDE };
		inline static KeyName KP_MULTIPLY{ KeyCode::KP_MULTIPLY };
		inline static KeyName KP_SUBTRACT{ KeyCode::KP_SUBTRACT };
		inline static KeyName KP_ADD{ KeyCode::KP_ADD };
		inline static KeyName KP_ENTER{ KeyCode::KP_ENTER };
		inline static KeyName KP_EQUAL{ KeyCode::KP_EQUAL };
		inline static KeyName LEFT_SHIFT{ KeyCode::LEFT_SHIFT };
		inline static KeyName LEFT_CONTROL{ KeyCode::LEFT_CONTROL };
		inline static KeyName LEFT_ALT{ KeyCode::LEFT_ALT };
		inline static KeyName LEFT_SUPER{ KeyCode::LEFT_SUPER };
		inline static KeyName RIGHT_SHIFT{ KeyCode::RIGHT_SHIFT };
		inline static KeyName RIGHT_CONTROL{ KeyCode::RIGHT_CONTROL };
		inline static KeyName RIGHT_ALT{ KeyCode::RIGHT_ALT };
		inline static KeyName RIGHT_SUPER{ KeyCode::RIGHT_SUPER };
		inline static KeyName MENU{ KeyCode::MENU };
		inline static KeyName MOUSE_LEFT{ KeyCode::MOUSE_LEFT };
		inline static KeyName MOUSE_RIGHT{ KeyCode::MOUSE_RIGHT };
		inline static KeyName MOUSE_MIDDLE{ KeyCode::MOUSE_MIDDLE };

	};

	struct STATE {
		inline static KeyState UP{ StateCode::UP };
		inline static KeyState DOWN{ StateCode::DOWN };
	};

	struct ACTION {
		inline static KeyAction RELEASE{ ActionCode::RELEASE };
		inline static KeyAction PRESS{ ActionCode::PRESS };
		inline static KeyAction REPEAT{ ActionCode::REPEAT };
	};

	struct CURSOR {
		inline static const CursorPosition X{ offsetof(Cursor,x) };
		inline static const CursorPosition Y{ offsetof(Cursor,y) };
	};



	static double now();
	static double delta();

	static bool isKeyDown(KeyName& key);
	static bool isKeyUp(KeyName& key);

	static bool isKeyPressed(KeyName& key);
	static bool isKeyReleased(KeyName& key);

private:
	static void keyHandler(GLFWwindow* window, KeyCode key, ActionCode action);

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double x, double y);
	static void scroll_callback(GLFWwindow* window, double x, double y);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	//static void char_callback(GLFWwindow* window, unsigned int codepoint);
	//static void window_focus_callback(GLFWwindow* window, int focused);

	inline static std::vector<Button> buttonEvents;
	inline static std::unordered_map<KeyCode, StateCode> buttonStates;
	inline static std::vector<Cursor> cursorEvents;
	inline static std::vector<Wheel> wheelEvents;
	
	static std::future<void> catchEvents;
	//static std::unordered_map<Event*,Window*> windowContexts;
	static Window* window;

	static bool startup;

	static double lastPollTime;
	static double currentPollTime;

	inline static bool firstPoll = true;
};

//Create own value type to make compact position checking

/*
	Have the ID as return value, but allow that ID or Type to be used in conjunction with Time and other things such as Event::Key(Event::KEY::A) &&
	Event::Key(Event::KEY::A) returning type with && operator (May not work due to not being able to return a value if no such operator is done.
	Maybe Event::Check << (Conditions here)
	Where Check << basically just runs a function to get true or false through using a interfaced function
*/