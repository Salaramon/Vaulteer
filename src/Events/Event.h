#pragma once

#include <unordered_set>
#include <chrono>
#include <utility>
#include <glm/glm.hpp>
#include <functional>

#include "API/Window.h"

//namespace Kyse {


// Main event system handler

class Event {
public:
	template <auto fn>
	struct GenericWrapper {};


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
		glfwSetWindowCloseCallback(rawWindow, window_close_callback);
	}


	static long double now() {
		std::chrono::duration<long double, std::ratio<60>> duration = std::chrono::high_resolution_clock::now().time_since_epoch();
		long double current = duration.count() - timeAtEventInitialization;

		return current;
	}

	static glm::vec2 cursor() {
		double x, y;
		glfwGetCursorPos(Window::getRawWindow(), &x, &y);

		return glm::vec2(x, y);
	}

	static KeyState state(KeyboardKey keyboardKey) {
		int key = glfwGetKey(Window::getRawWindow(), static_cast<int>(keyboardKey));

		return static_cast<KeyState>(key);
	}

	static KeyState state(MouseKey mouseKey) {
		int key = glfwGetMouseButton(Window::getRawWindow(), static_cast<int>(mouseKey));

		return static_cast<KeyState>(key);
	}

	static bool isUp(KeyboardKey key) {
		return state(key) == KeyState::UP;
	}

	static bool isDown(KeyboardKey key) {
		return state(key) == KeyState::DOWN;
	}

	static bool isUp(MouseKey key) {
		return state(key) == KeyState::UP;
	}

	static bool isDown(MouseKey key) {
		return state(key) == KeyState::DOWN;
	}


	static void poll() {
		glfwPollEvents();
	}


	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
		if (Window::isFocused()) {
			auto eKey = static_cast<MouseKey>(button);
			auto eAction = static_cast<KeyAction>(action);

			MouseButton eButton = {
				.key = eKey,
				.action = eAction
			};

			MouseButtonEvent e(eButton);
			eventCallbackFn(e);
		}
	}

	static void cursor_position_callback(GLFWwindow* window, double x, double y) {
		if (Window::isFocused()) {
			glm::vec2 mousePosition = glm::vec2(x, y);
			glm::vec2 mouseDelta = mousePosition - previousPolledMousePosition;

			Motion motion = {
				.position = glm::vec2(x, y),
				.delta = mousePosition - previousPolledMousePosition
			};
			
			MouseMoveEvent e(motion);
			eventCallbackFn(e);

			//Keep this at the end of this callback
			previousPolledMousePosition = mousePosition;
		}
	}

	static void scroll_callback(GLFWwindow* window, double x, double y) {
		if (Window::isFocused()) {
			glm::vec2 scrollChange = glm::vec2(x, y);
			
			MouseScrollEvent e(scrollChange);
			eventCallbackFn(e);
		}
	}

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (Window::isFocused()) {
			auto eKey = static_cast<KeyboardKey>(key);
			auto eAction = static_cast<KeyAction>(action);

			KeyboardButton eButton = {
				.key = eKey,
				.action = eAction
			};

			KeyboardButtonEvent e(eButton);
			eventCallbackFn(e);
		}
	}
	
	static void window_focus_callback(GLFWwindow* window, int focused) {
		WindowFocusEvent e(focused);
		eventCallbackFn(e);
	}
	
	static void window_close_callback(GLFWwindow* window) {
		WindowCloseEvent e;
		eventCallbackFn(e);
	}


	static void setEventCallback(std::function<void(BaseEvent&)> eventCallbackFn) {
		Event::eventCallbackFn = std::move(eventCallbackFn);
	}

private:
	inline static std::function<void(BaseEvent&)> eventCallbackFn;

	inline static glm::vec2 previousPolledMousePosition;
	inline static long double timeAtEventInitialization;

};

class EventDispatcher {
public:
	EventDispatcher(BaseEvent& e) : event(e) {}

	template <typename T, typename F>
	bool dispatch(const F& func) {
		if (event.getEventType() == T::getStaticType()) {
			event.handled |= func(static_cast<T&>(event));
			return true;
		}
		return false;
	}

	BaseEvent& event;
};

//}
