#pragma once

#include <unordered_set>
#include <chrono>

#include "API/Window.h"


namespace Kyse {


	class Event
	{
	public:
		template<auto fn>
		struct GenericWrapper {};

#define FORWARD_THIS [this]<class EventType, auto fn>(EventType& e, Event::GenericWrapper<fn> fnWrap) -> bool { return (this->*fn)(e); }


		static void initialize() {
			std::chrono::duration<long double, std::ratio<60>> duration = std::chrono::high_resolution_clock::now().time_since_epoch();
			timeAtEventInitialization = duration.count();

			double x, y;
			glfwGetCursorPos(Window::getRawWindow(), &x, &y);
			previousPolledMousePosition = glm::vec2(x, y);

			GLFWwindow* rawWindow = Window::getRawWindow();

			glfwSetMouseButtonCallback(rawWindow, mouse_button_callback);
			glfwSetCursorPosCallback(rawWindow, cursor_position_callback);
			glfwSetScrollCallback(rawWindow, scroll_callback);
			glfwSetKeyCallback(rawWindow, key_callback);
			glfwSetWindowFocusCallback(rawWindow, window_focus_callback);
		}



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
			RELEASE = 0,
			PRESS = 1,
			REPEAT = 2,
		};

		enum class KeyState {
			UP = 0,
			DOWN = 1
		};



		static long double now() {
			std::chrono::duration<long double, std::ratio<60>> duration = std::chrono::high_resolution_clock::now().time_since_epoch();
			long double current = duration.count() - timeAtEventInitialization;

			return current;
		};

		static glm::vec2 cursor() {
			double x, y;
			glfwGetCursorPos(Window::getRawWindow(), &x, &y);

			return glm::vec2(x, y);
		};

		static KeyState state(KeyboardKey keyboardKey) {
			int key = glfwGetKey(Window::getRawWindow(), static_cast<int>(keyboardKey));

			return static_cast<KeyState>(key);
		};

		static KeyState state(MouseKey mouseKey) {
			int key = glfwGetMouseButton(Window::getRawWindow(), static_cast<int>(mouseKey));

			return static_cast<KeyState>(key);
		};


		static long double delta(long double previousTime) {
			return now() - previousTime;
		}

		static glm::vec2 delta(glm::vec2 previousPosition) {
			double x, y;
			glfwGetCursorPos(Window::getRawWindow(), &x, &y);

			return glm::vec2(x, y) - previousPosition;
		};


		struct KeyboardButton {
		public:
			KeyboardKey key;
			KeyAction action;
		};

		struct MouseButton {
		public:
			MouseKey key;
			KeyAction action;
		};

		struct Motion {
		public:
			glm::vec2 position;
			glm::vec2 delta;
		};



		template<class Derived>
		struct BaseEvent {
		private:
			template<class... Args>
			struct Parameter {
				using Tail = std::tuple_element_t<sizeof...(Args) - 1, std::tuple<Args...>>;
				using Head = std::tuple_element_t<0, std::tuple<Args...>>;

				template<size_t n>
				using Get = std::tuple_element_t<n, std::tuple<Args...>>;
			};

			template<class F>
			struct FunctionType {};

			template<class R, class... Args>
			struct FunctionType<R(Args...)> {
			public:
				using Return = R;
				using Parameters = std::tuple<Args...>;
			};

			template<class R, class C, class... Args>
			struct FunctionType<R(C::*)(Args...)> {
			public:
				using Return = R;
				using Parameters = Parameter<Args...>;
				using Class = C;
			};

		public:
			bool Handled = false;



			template<auto fn, class F>
			bool dispatch(const F& bypass_protection) {
				if (std::is_same_v<std::remove_reference_t<typename FunctionType<decltype(fn)>::Parameters::Head>, Derived>) {
					return bypass_protection(reinterpret_cast<Derived&>(*this), GenericWrapper<fn>());
				}

				return false;
			}

			template<auto fn>
			bool dispatch(typename FunctionType<decltype(fn)>::Class* object) {
				if (std::is_same_v<std::remove_reference_t<typename FunctionType<decltype(fn)>::Parameters::Head>, Derived>) {
					return (object->*fn)(reinterpret_cast<Derived&>(*this));
				}

				return false;
			}
		};

		struct KeyboardButtonEvent : public BaseEvent<KeyboardButtonEvent> {
		public:
			KeyboardButtonEvent(KeyboardButton key) : button(button) {}
			KeyboardButton get() const { return button; }

		private:
			KeyboardButton button;
		};

		struct MouseButtonEvent : public BaseEvent<MouseButtonEvent> {
		public:
			MouseButtonEvent(MouseKey key) : key(key) {}
			MouseKey get() const { return key; }

		private:
			MouseKey key;
		};

		struct MouseWheelEvent : public BaseEvent<MouseWheelEvent> {
		public:
			MouseWheelEvent(glm::vec2 mag) : mag(mag) {}
			glm::vec2 get() const { return mag; }

		private:
			glm::vec2 mag;
		};

		struct MouseMoveEvent : public BaseEvent<MouseMoveEvent> {
		public:
			MouseMoveEvent(Motion motion) : motion(motion) {}
			Motion get() const { return motion; }

		private:
			Motion motion;
		};

		struct WindowCloseEvent : public BaseEvent<WindowCloseEvent> {
		public:
			bool get() const { return true; }
		};

		struct WindowFocusEvent : public BaseEvent<WindowFocusEvent> {
		public:
			WindowFocusEvent(bool focused) : focused(focused) {}
			bool get() const { return true; }

		private:
			bool focused;
		};


	private:

		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
			if (windowFocused) {
				MouseKey eKey = static_cast<MouseKey>(button);
				KeyAction eAction = static_cast<KeyAction>(action);

				MouseButton eButton = {
					.key = eKey,
					.action = eAction
				};

				//onEvent(MouseButtonEvent(eButton));
			}
		}

		static void cursor_position_callback(GLFWwindow* window, double x, double y) {
			if (windowFocused) {
				glm::vec2 mousePosition = glm::vec2(x, y);
				glm::vec2 mouseDelta = mousePosition - previousPolledMousePosition;

				Motion motion = {
					.position = glm::vec2(x, y),
					.delta = mousePosition - previousPolledMousePosition
				};

				//onEvent(MouseButtonEvent(motion));


				//Keep this at the end of this callback
				previousPolledMousePosition = mousePosition;
			}
		}

		static void scroll_callback(GLFWwindow* window, double x, double y) {
			if (windowFocused) {
				glm::vec2 scrollChange = glm::vec2(x, y);

				//onEvent(MouseWheelEvent(scrollChange));
			}
		}

		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
			if (windowFocused) {
				KeyboardKey eKey = static_cast<KeyboardKey>(key);
				KeyAction eAction = static_cast<KeyAction>(action);

				KeyboardButton eButton = {
					.key = eKey,
					.action = eAction
				};

				//onEvent(KeyboardButtonEvent(eButton));
			}
		}

		static void window_focus_callback(GLFWwindow* window, int focused) {
				windowFocused = focused;
		}


		inline static glm::vec2 previousPolledMousePosition;
		inline static bool windowFocused;
		inline static long double timeAtEventInitialization;
		

	};

}