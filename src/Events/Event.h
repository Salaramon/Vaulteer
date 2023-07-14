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

		glfwSetWindowCloseCallback(rawWindow, window_close_callback);
		glfwSetWindowPosCallback(rawWindow, window_position_callback);
		glfwSetWindowMaximizeCallback(rawWindow, window_maximize_callback);
		glfwSetWindowFocusCallback(rawWindow, window_focus_callback);
		glfwSetFramebufferSizeCallback(rawWindow, window_resize_callback);
		glfwSetWindowIconifyCallback(rawWindow, window_iconify_callback);
	}                                               

	static long double now() {
		std::chrono::duration<long double, std::ratio<60>> duration = std::chrono::high_resolution_clock::now().time_since_epoch();
		long double current = duration.count() - timeAtEventInitialization;

		return current;
	}

	/*
	 * Note: in mode GLFW_CURSOR_DISABLED, this returns a virtual mouse position that reflects
	 * how far the cursor has moved from the center of a coordinate system that starts at (0,0).
	 */
	static glm::vec2 cursorPos() {
		double x, y;
		glfwGetCursorPos(Window::getRawWindow(), &x, &y);

		return glm::vec2(x, y);
	}

	 static glm::vec2 delta(glm::vec2 previousPosition) { 
	     double x, y;                                     
	     glfwGetCursorPos(Window::getRawWindow(), &x, &y);
	                                                      
	     return glm::vec2(x, y) - previousPosition;       
	 };    

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

		if (queuedResizeEvent != nullptr) {
			eventCallbackFn(*queuedResizeEvent);
			queuedResizeEvent.reset();
		}
	}


	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
		if (Window::focused) {
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
		if (Window::focused) {
			glm::vec2 mousePosition = glm::vec2(x, y);
			glm::vec2 mouseDelta = mousePosition - previousPolledMousePosition;

			Motion motion = {
				.position = glm::vec2(x, y),
				.delta = mousePosition - previousPolledMousePosition
			};
			
 			MouseMoveEvent e(motion);
			if (!resizeSkipMousePoll)
				eventCallbackFn(e);
			else
				resizeSkipMousePoll = false;

			//Keep this at the end of this callback
			previousPolledMousePosition = mousePosition;
		}
	}

	static void scroll_callback(GLFWwindow* window, double x, double y) {
		if (Window::focused) {
			glm::vec2 scrollChange = glm::vec2(x, y);
			
			MouseScrollEvent e(scrollChange);
			eventCallbackFn(e);
		}
	}

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (Window::focused) {
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
	
	static void window_close_callback(GLFWwindow* window) {
		WindowCloseEvent e;
		eventCallbackFn(e);
	}

	static void window_fullscreen_callback(GLFWwindow* window, int fullscreen) {
		WindowFullscreenEvent e(fullscreen);
		eventCallbackFn(e);
	}

	static void window_maximize_callback(GLFWwindow* window, int maximized) {
		WindowMaximizeEvent e(maximized);
		eventCallbackFn(e);
	}

	static void window_position_callback(GLFWwindow* window, int xpos, int ypos) {
		WindowPositionEvent e(xpos, ypos);
		eventCallbackFn(e);
	}
	
	static void window_focus_callback(GLFWwindow* window, int focused) {
		WindowFocusEvent e(focused);
		eventCallbackFn(e);
	}

	// -- special resize state --
	// used to handle window resize after rendered frame to prevent state errors
	inline static std::unique_ptr<WindowResizeEvent> queuedResizeEvent;
	// used to skip mouse polling after resize to prevent unexpected camera jump
	inline static bool resizeSkipMousePoll;

	static void window_resize_callback(GLFWwindow* window, int width, int height) {
		//WindowResizeEvent e(width, height);
		//eventCallbackFn(e);
		if (width == 0 || height == 0)
			window_iconify_callback(window, true);
		else 
			queuedResizeEvent = std::make_unique<WindowResizeEvent>(width, height);

		resizeSkipMousePoll = true;
	}

	static void window_iconify_callback(GLFWwindow* window, int iconified) {
		// TODO this might not be needed
		glfwMakeContextCurrent(window);
		//eventCallbackFn(e);
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
