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

	enum class enumKEY {
		UNKNOWN = -1,
		START_OF_KEYBOARDKEYS = enumKEY::UNKNOWN,
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

		START_OF_MOUSEKEYS = enumKEY::MENU,
		MOUSE_LEFT = 349,
		MOUSE_RIGHT = 350,
		MOUSE_MIDDLE = 351
	};

	enum class enumACTION {
		RELEASE = 0,
		PRESS = 1,
		REPEAT = 2,
	};

	enum class enumSTATE {
		UP = 0,
		DOWN = 1
	};

	struct Button {
		enumKEY key;
		enumACTION action;
	};
	struct Cursor {
		double_t x;
		double_t y;
	};
	struct Wheel {
		double_t ScrollX;
		double_t ScrollY;
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
		KeyAction(enumACTION actionID) : actionID(actionID) {}
		
		enumACTION actionID;

		std::vector<Button> possibleEvents;
	};

	struct KeyState {
	public:
		KeyState(enumSTATE stateID) : stateID(stateID) {}

		enumSTATE stateID;
	};

	struct KeyName {
	public:
		KeyName(enumKEY keyID) : keyID(keyID){}

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

		enumKEY keyID;
	};

	struct KEY {
		inline static KeyName UNKNOWN{ enumKEY::UNKNOWN };
		inline static KeyName SPACE{ enumKEY::SPACE };
		inline static KeyName APOSTROPHE{ enumKEY::APOSTROPHE };
		inline static KeyName COMMA{ enumKEY::COMMA };
		inline static KeyName MINUS{ enumKEY::MINUS };
		inline static KeyName PERIOD{ enumKEY::PERIOD };
		inline static KeyName SLASH{ enumKEY::SLASH };
		inline static KeyName _0{ enumKEY::_0 };
		inline static KeyName _1{ enumKEY::_1 };
		inline static KeyName _2{ enumKEY::_2 };
		inline static KeyName _3{ enumKEY::_3 };
		inline static KeyName _4{ enumKEY::_4 };
		inline static KeyName _5{ enumKEY::_5 };
		inline static KeyName _6{ enumKEY::_6 };
		inline static KeyName _7{ enumKEY::_7 };
		inline static KeyName _8{ enumKEY::_8 };
		inline static KeyName _9{ enumKEY::_9 };
		inline static KeyName SEMICOLON{ enumKEY::SEMICOLON };
		inline static KeyName EQUAL{ enumKEY::EQUAL };
		inline static KeyName A{ enumKEY::A };
		inline static KeyName B{ enumKEY::B };
		inline static KeyName C{ enumKEY::C };
		inline static KeyName D{ enumKEY::D };
		inline static KeyName E{ enumKEY::E };
		inline static KeyName F{ enumKEY::F };
		inline static KeyName G{ enumKEY::G };
		inline static KeyName H{ enumKEY::H };
		inline static KeyName I{ enumKEY::I };
		inline static KeyName J{ enumKEY::J };
		inline static KeyName K{ enumKEY::K };
		inline static KeyName L{ enumKEY::L };
		inline static KeyName M{ enumKEY::M };
		inline static KeyName N{ enumKEY::N };
		inline static KeyName O{ enumKEY::O };
		inline static KeyName P{ enumKEY::P };
		inline static KeyName Q{ enumKEY::Q };
		inline static KeyName R{ enumKEY::R };
		inline static KeyName S{ enumKEY::S };
		inline static KeyName T{ enumKEY::T };
		inline static KeyName U{ enumKEY::U };
		inline static KeyName V{ enumKEY::V };
		inline static KeyName W{ enumKEY::W };
		inline static KeyName X{ enumKEY::X };
		inline static KeyName Y{ enumKEY::Y };
		inline static KeyName Z{ enumKEY::Z };
		inline static KeyName LEFT_BRACKET{ enumKEY::LEFT_BRACKET };
		inline static KeyName BACKSLASH{ enumKEY::BACKSLASH };
		inline static KeyName RIGHT_BRACKET{ enumKEY::RIGHT_BRACKET };
		inline static KeyName GRAVE_ACCENT{ enumKEY::GRAVE_ACCENT };
		inline static KeyName WORLD_1{ enumKEY::WORLD_1 };
		inline static KeyName WORLD_2{ enumKEY::WORLD_2 };
		inline static KeyName ESCAPE{ enumKEY::ESCAPE };
		inline static KeyName ENTER{ enumKEY::ENTER };
		inline static KeyName TAB{ enumKEY::TAB };
		inline static KeyName BACKSPACE{ enumKEY::BACKSPACE };
		inline static KeyName INSERT{ enumKEY::INSERT };
		inline static KeyName DELETE{ enumKEY::DELETE };
		inline static KeyName RIGHT{ enumKEY::RIGHT };
		inline static KeyName LEFT{ enumKEY::LEFT };
		inline static KeyName DOWN{ enumKEY::DOWN };
		inline static KeyName UP{ enumKEY::UP };
		inline static KeyName PAGE_UP{ enumKEY::PAGE_UP };
		inline static KeyName PAGE_DOWN{ enumKEY::PAGE_DOWN };
		inline static KeyName HOME{ enumKEY::HOME };
		inline static KeyName END{ enumKEY::END };
		inline static KeyName CAPS_LOCK{ enumKEY::CAPS_LOCK };
		inline static KeyName SCROLL_LOCK{ enumKEY::SCROLL_LOCK };
		inline static KeyName NUM_LOCK{ enumKEY::NUM_LOCK };
		inline static KeyName PRINT_SCREEN{ enumKEY::PRINT_SCREEN };
		inline static KeyName PAUSE{ enumKEY::PAUSE };
		inline static KeyName F1{ enumKEY::F1 };
		inline static KeyName F2{ enumKEY::F2 };
		inline static KeyName F3{ enumKEY::F3 };
		inline static KeyName F4{ enumKEY::F4 };
		inline static KeyName F5{ enumKEY::F5 };
		inline static KeyName F6{ enumKEY::F6 };
		inline static KeyName F7{ enumKEY::F7 };
		inline static KeyName F8{ enumKEY::F8 };
		inline static KeyName F9{ enumKEY::F9 };
		inline static KeyName F10{ enumKEY::F10 };
		inline static KeyName F11{ enumKEY::F11 };
		inline static KeyName F12{ enumKEY::F12 };
		inline static KeyName F13{ enumKEY::F13 };
		inline static KeyName F14{ enumKEY::F14 };
		inline static KeyName F15{ enumKEY::F15 };
		inline static KeyName F16{ enumKEY::F16 };
		inline static KeyName F17{ enumKEY::F17 };
		inline static KeyName F18{ enumKEY::F18 };
		inline static KeyName F19{ enumKEY::F19 };
		inline static KeyName F20{ enumKEY::F20 };
		inline static KeyName F21{ enumKEY::F21 };
		inline static KeyName F22{ enumKEY::F22 };
		inline static KeyName F23{ enumKEY::F23 };
		inline static KeyName F24{ enumKEY::F24 };
		inline static KeyName F25{ enumKEY::F25 };
		inline static KeyName KP_0{ enumKEY::KP_0 };
		inline static KeyName KP_1{ enumKEY::KP_1 };
		inline static KeyName KP_2{ enumKEY::KP_2 };
		inline static KeyName KP_3{ enumKEY::KP_3 };
		inline static KeyName KP_4{ enumKEY::KP_4 };
		inline static KeyName KP_5{ enumKEY::KP_5 };
		inline static KeyName KP_6{ enumKEY::KP_6 };
		inline static KeyName KP_7{ enumKEY::KP_7 };
		inline static KeyName KP_8{ enumKEY::KP_8 };
		inline static KeyName KP_9{ enumKEY::KP_9 };
		inline static KeyName KP_DECIMAL{ enumKEY::KP_DECIMAL };
		inline static KeyName KP_DIVIDE{ enumKEY::KP_DIVIDE };
		inline static KeyName KP_MULTIPLY{ enumKEY::KP_MULTIPLY };
		inline static KeyName KP_SUBTRACT{ enumKEY::KP_SUBTRACT };
		inline static KeyName KP_ADD{ enumKEY::KP_ADD };
		inline static KeyName KP_ENTER{ enumKEY::KP_ENTER };
		inline static KeyName KP_EQUAL{ enumKEY::KP_EQUAL };
		inline static KeyName LEFT_SHIFT{ enumKEY::LEFT_SHIFT };
		inline static KeyName LEFT_CONTROL{ enumKEY::LEFT_CONTROL };
		inline static KeyName LEFT_ALT{ enumKEY::LEFT_ALT };
		inline static KeyName LEFT_SUPER{ enumKEY::LEFT_SUPER };
		inline static KeyName RIGHT_SHIFT{ enumKEY::RIGHT_SHIFT };
		inline static KeyName RIGHT_CONTROL{ enumKEY::RIGHT_CONTROL };
		inline static KeyName RIGHT_ALT{ enumKEY::RIGHT_ALT };
		inline static KeyName RIGHT_SUPER{ enumKEY::RIGHT_SUPER };
		inline static KeyName MENU{ enumKEY::MENU };
		inline static KeyName MOUSE_LEFT{ enumKEY::MOUSE_LEFT };
		inline static KeyName MOUSE_RIGHT{ enumKEY::MOUSE_RIGHT };
		inline static KeyName MOUSE_MIDDLE{ enumKEY::MOUSE_MIDDLE };

	};

	struct STATE {
		inline static KeyState UP{ enumSTATE::UP };
		inline static KeyState DOWN{ enumSTATE::DOWN };
	};

	struct ACTION {
		inline static KeyAction RELEASE{ enumACTION::RELEASE };
		inline static KeyAction PRESS{ enumACTION::PRESS };
		inline static KeyAction REPEAT{ enumACTION::REPEAT };
	};

	

	static void keyHandler(GLFWwindow* window, enumKEY key, enumACTION action);

	struct doubleConverter {
	public:
		doubleConverter(int value) : value(static_cast<double_t>(value)) {}
		operator double_t() { return value; }
		double_t value;
	};

	struct CursorPosition {
	public:
		CursorPosition(size_t offset) : offset(offset) {}
		//bool operator<(doubleConverter axis) { return check(axis, std::less<bool>()); }
		//bool operator>(doubleConverter axis) { return check(axis, std::greater<bool>()); }
		//bool operator<=(doubleConverter axis) { return check(axis, std::less_equal<bool>()); }
		//bool operator>=(doubleConverter axis) { return check(axis, std::greater_equal<bool>()); }

		friend BooleanCheck operator<(const double_t lhs, const CursorPosition& rhs);
		friend BooleanCheck operator>(const double_t lhs, const CursorPosition& rhs);
		friend BooleanCheck operator<=(const double_t lhs, const CursorPosition& rhs);
		friend BooleanCheck operator>=(const double_t lhs, const CursorPosition& rhs);
		friend BooleanCheck operator<(const CursorPosition& lhs, const double_t rhs);
		friend BooleanCheck operator>(const CursorPosition& lhs, const double_t rhs);
		friend BooleanCheck operator<=(const CursorPosition& lhs, const double_t rhs);
		friend BooleanCheck operator>=(const CursorPosition& lhs, const double_t rhs);

		template<class Func>
		bool check(double_t axis, Func func) const  {
			for (auto& pos : cursorEvents) {
				double_t cur = *(reinterpret_cast<double_t*>((&pos)) + (offset / sizeof(double_t)));
				if (func(cur, axis)) {
					return true;
				}
			}
			return false;
		}

		double_t get() const{
			if (!cursorEvents.empty()) {
				double_t cur = *(reinterpret_cast<double_t*>((&cursorEvents.back())) + (offset / sizeof(double_t)));
				return cur;
			}
			else {
				return 0;
			}
		}

		double_t delta() const {
			if (!cursorEvents.empty()) {
				double_t front = *(reinterpret_cast<double_t*>((&cursorEvents.front())) + (offset / sizeof(double_t)));
				double_t back = *(reinterpret_cast<double_t*>((&cursorEvents.back())) + (offset / sizeof(double_t)));
				return back - front;
			}
			else {
				return 0;
			}
		}

		const size_t offset;
	};

	struct CURSOR {
		inline static const CursorPosition X{ offsetof(Cursor,x) };
		inline static const CursorPosition Y{ offsetof(Cursor,y) };
	};

	static double_t now();
	static double_t delta();

private:

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double x, double y);
	static void scroll_callback(GLFWwindow* window, double x, double y);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	//static void char_callback(GLFWwindow* window, unsigned int codepoint);
	//static void window_focus_callback(GLFWwindow* window, int focused);

	inline static std::vector<Button> buttonEvents;
	inline static std::unordered_map<enumKEY, enumSTATE> buttonStates;
	inline static std::vector<Cursor> cursorEvents;
	inline static std::vector<Wheel> wheelEvents;
	
	static std::future<void> catchEvents;
	//static std::unordered_map<Event*,Window*> windowContexts;
	static Window* window;

	static bool startup;

	static double_t lastPollTime;
	static double_t currentPollTime;
};

//Create own value type to make compact position checking

/*
	Have the ID as return value, but allow that ID or Type to be used in conjunction with Time and other things such as Event::Key(Event::KEY::A) &&
	Event::Key(Event::KEY::A) returning type with && operator (May not work due to not being able to return a value if no such operator is done.
	Maybe Event::Check << (Conditions here)
	Where Check << basically just runs a function to get true or false through using a interfaced function
*/